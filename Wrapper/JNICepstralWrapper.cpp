/*
 * JNICepstralWrapper.c
 * @author: N Krishna
 * State University of New York at Stony Brook
 */


/* 
 * JNICepstral.h is the header file generated using javah. 
 * It includes the C prototypes for the JNICepstral functions
 */
#include "cepstral_JNICepstral.h"

/* 
 *Include Libraries 
 */
#include <swift.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Following are for windows */
/*#ifdef _WIN32
#include <dos.h>
#include <windows.h>
#include <conio.h>
#endif

/* 
 * Define macros for boolean values 
 */
#define TRUE 1
#define FALSE 0

/*
	Define Macros for ports. 
*/
#define CMD_PORT 0
#define ARTICLE_PORT 1

swift_result_t handle_events(swift_event *event, swift_event_t type, void *udata);

static int event_count = 0;
static int textpos = 0;


/*
	Variables Definition
*/
JNIEnv* g_env = NULL;
jclass g_jcls = NULL;
jobject g_jobj = NULL;
JavaVM* g_pJvm = NULL;

swift_engine *engine;
swift_port *cmd_port = NULL;
swift_port *article_port = NULL;
swift_background_t cmd_stream;
swift_background_t article_stream;

const char *_article;
const char *_command;
const char *_voice;
int event_mask;
int debug = FALSE;
int key_down();
/**************************INITIALIZATION******************************/
/*
	Before calling any functions in the SWIFT API, we must open an engine and a port.
	It is through this port, that text-to-speech conversion is performed by the engine.
*/

JNIEXPORT jboolean JNICALL Java_cepstral_JNICepstral_initialize
  (JNIEnv *jnienv, jobject jobj)
{
    int success = TRUE;

   /* Open the Swift TTS Engine */
   if ((engine = swift_engine_open(NULL)) == NULL)
	{
        success = FALSE;
        goto all_done;
    	}

    /* Open a Swift Port through which to make TTS calls */
    if ((article_port = swift_port_open(engine, NULL)) == NULL)
	{
        success = FALSE;  
        goto all_done;
    	}

    event_mask = ~SWIFT_EVENT_AUDIO;
    
	all_done:
    return ( success );
}

/**************************SHUTDOWN******************************/
/*
	Once the system has finished speaking, the open port must be closed.
*/

JNIEXPORT jboolean JNICALL Java_cepstral_JNICepstral_shutdown(
JNIEnv * jnienv, jobject jobj)
{
    if (NULL != article_port) swift_port_close(article_port);
    if (NULL != engine) swift_engine_close(engine);
    return ( TRUE );
}



/**************************CHANGE VOICE******************************/
/*
	This function is used to switch between the available voices.
*/

JNIEXPORT jboolean JNICALL Java_cepstral_JNICepstral_changeVoice
(JNIEnv *env, jobject jo, jstring voicer)
{
	/*
		Get ASCII representation of environment variable name
	*/
	_voice = env->GetStringUTFChars (voicer, 0);

/*
	swift_port_set_voice_by_name loads the voice referenced by
	_voice and sets it as the port's current voice.
*/

		swift_port_set_voice_by_name(article_port,_voice);
		return (TRUE);
	
	/* 
		Release memory used to hold ASCII representation
	*/
	env->ReleaseStringUTFChars (voicer, _voice);

}

/**************************SAY ARTICLE******************************/
/*
	This function is used to synthesize the text at the article_port
*/

JNIEXPORT jboolean JNICALL Java_cepstral_JNICepstral_sayArticle
(JNIEnv *env, jobject jo, jstring article)
{
	/*
		If another job is currently Running, or the current job is Paused, 
		then we must wait for the previous job to complete or for the
		user to resume the speech
	*/
	
	if (SWIFT_STATUS_RUNNING == swift_port_status(article_port, article_stream) ||
        SWIFT_STATUS_PAUSED == swift_port_status(article_port, article_stream))
	{

        swift_port_wait(article_port, article_stream);
    }

/*
		Get ASCII representation of environment variable name
	*/
   _article = env->GetStringUTFChars (article, 0);

    /*
		Sending the text to the cepstral port:
	*/
    swift_port_speak_text(article_port, _article, 0, NULL, &article_stream, NULL);
	
    /* 
		Release memory used to hold ASCII representation
	*/
	env->ReleaseStringUTFChars (article, _article);
    return ( TRUE );
}

/**************************CANCEL******************************/
/*
	This function is used to cancel the currently executing job
*/
JNIEXPORT jint JNICALL Java_cepstral_JNICepstral_cancel
(JNIEnv *env, jobject jo, jint port)
{
    int success = FALSE;

    int status;
	/*
		Check to see if the currently executing job is on the cmd_port
	*/
	// XXX: Modified logic to cancel current job always
	status = swift_port_stop(article_port, SWIFT_ASYNC_CURRENT, SWIFT_EVENT_NOW);
	/*{ // Else the currently executing job is on the article_port
        status = swift_port_status(article_port, article_stream);
		if (SWIFT_STATUS_PAUSED != status) 
		{
            swift_port_pause(article_port, article_stream, SWIFT_EVENT_NOW);
		    status = swift_port_status(article_port, article_stream); 
		}
	}*/
		// This function returns the status of the port
    return ( status );
}

/**************************SET VOLUME******************************/
/*
	This function is used to set the volume.
	The integer parameter v holds the value to which we have to set the volume.
*/
JNIEXPORT jint JNICALL Java_cepstral_JNICepstral_setVolume(JNIEnv *env, jobject jo, jint v)
{
  int success = FALSE;
      success = TRUE;
  	
	/*
		Parameter Name: audio/volume:Integer
		Volume multiplication factor (Percentage: default is 100)
	*/
    swift_port_set_param(article_port, "audio/volume", swift_val_int(v), article_stream);
    return ( v );
}

/**************************SET SPEED******************************/
/*
	This function is used to set the rate of speech.
	The integer parameter speed holds the value to which we have to set the speed.
*/
JNIEXPORT jint JNICALL Java_cepstral_JNICepstral_setSpeed(JNIEnv *env, jobject jo, jint speed)
{
   int success = FALSE;
   success = TRUE;

	/*
		Parameter Name: speech/rate:Integer
		Speaking Rate in WPM (Default: 170 WPM)
	*/
	if (swift_port_status(article_port, article_stream) >= SWIFT_STATUS_RUNNING)        
	{

        swift_port_wait(article_port, article_stream);
    }
    swift_port_set_param(article_port, "speech/rate", swift_val_int(speed), article_stream);  
    return ( speed );
}

/**************************SET PITCH******************************/
/*
	This function is used to set the pitch.
	The double parameter speed holds the value to which we have to set the pitch.
*/
JNIEXPORT jdouble JNICALL Java_cepstral_JNICepstral_setPitch(JNIEnv *env, jobject jo, jdouble pitch)
{
   //int success = FALSE;

    //success = TRUE;

	/*
		Parameter Name: speech/pitch/shift:Float
		Frequency Multiplier ( Default is 1.0)
	*/

    swift_port_set_param(article_port, "speech/pitch/shift", swift_val_int(pitch), article_stream);  
    return ( pitch );
}

/**************************IS TEXT PLAYING******************************/
/*
	This function is used to check the SWIFT STATUS, and check
	if text is currently playing or is it paused/queued/stopped
*/
JNIEXPORT jboolean JNICALL Java_cepstral_JNICepstral_isTextPlaying(JNIEnv *env, jobject jo, jint port)
{

    int playing = FALSE;
	/* Check for CMD_PORT */
	/* Check for ARTICLE_PORT */
	if (port == ARTICLE_PORT) 
	{
		int status = swift_port_status(article_port, article_stream);
        if (status == SWIFT_STATUS_DONE) 
		{
            swift_port_stop(article_port, article_stream, SWIFT_EVENT_NOW);
			swift_port_wait(article_port, article_stream);    // block until complete
            status = swift_port_status(article_port, article_stream);
		}
        if (status >= SWIFT_STATUS_RUNNING)
		{
            playing = TRUE;
		}
	}
    return ( playing );
}

/**************************STOP TEXT******************************/
/*
	This function is used to stop the currently executing job
*/
JNIEXPORT jboolean JNICALL Java_cepstral_JNICepstral_stopText
(JNIEnv *env, jobject jo)
{
	swift_port_stop(article_port,article_stream, SWIFT_EVENT_NOW);
    return ( TRUE );
}

/**************************PAUSE ARTICLE******************************/
/*
	This function is used to pause the currently executing job
*/
JNIEXPORT jboolean JNICALL Java_cepstral_JNICepstral_pauseArticle
(JNIEnv *env, jobject jo)
{
    int success = FALSE;

	/* Checking to see if the job isnt already paused */

    if (SWIFT_STATUS_PAUSED != swift_port_status(article_port, article_stream))
	{

       swift_port_pause(article_port, article_stream, SWIFT_EVENT_NOW);
        success = TRUE;
    }	

    return ( success );
}

/**************************RESUME ARTICLE******************************/
/*
	This function is used to resume a paused job
*/

JNIEXPORT jboolean JNICALL Java_cepstral_JNICepstral_resumeArticle
(JNIEnv *env, jobject jo)
{
    int success = FALSE;

   /* Check to see if the job is currently paused */
    if (SWIFT_STATUS_PAUSED == swift_port_status(article_port, article_stream))
	{
        swift_port_pause(article_port, article_stream, SWIFT_EVENT_NOW);
        success = TRUE;
    }

    return ( success );
}

/**************************EVENT HANDLER******************************/

swift_result_t handle_events(swift_event *event, swift_event_t type, void *udata)
{
    float time_start, time_len;
    int   text_start, text_len, i;
    const char *type_name = "";
    char *text = "";
    char *trunc_text = NULL;
    char evt_count[10];

    event_count++;
    sprintf(evt_count,"%d",event_count);

    /* Get the string name of the event type */
    type_name = swift_event_type_get_name(type);
    /* Get the event times */
    swift_event_get_times(event, &time_start, &time_len);
    /* Get the event text */
    swift_event_get_text(event, &text);

    if ("" == text)
        text = "N/A";
    if (26 < strlen(text))
    {
        trunc_text = (char *)malloc(27);
        strncpy(trunc_text, text, 23);
        trunc_text[23] = '.';
        trunc_text[24] = '.';
        trunc_text[25] = '.';
        trunc_text[26] = 0;
    }

    if (debug) printf("%s", evt_count);
    for (i = strlen(evt_count); i<4; i++) {if (debug) printf(" ");}
    if (debug) printf("| %s", type_name);
    for (i = strlen(type_name); i<12; i++) {if (debug) printf(" ");}
    if (debug) printf("| %0.4f | %0.4f ", time_start, time_start + time_len);



    /* Get the event text positions, if any */
    if (!SWIFT_FAILED(swift_event_get_textpos(event, &text_start, &text_len))) {
        if (type == SWIFT_EVENT_WORD && SWIFT_STATUS_PAUSED != swift_port_status(article_port, article_stream)) textpos = text_start;
        char text_start_str[12];
        char text_end_str[12];
        sprintf(text_start_str, "%d", text_start);
        sprintf(text_end_str, "%d", text_start + text_len);

        if (debug) printf("| %s-%s ", text_start_str, text_end_str);
        for (i = (strlen(text_start_str)+strlen(text_end_str)+1); i<8; i++)
            if (debug) printf(" ");
    }
    else
        if (debug) printf("| N/A      ");


    if (trunc_text)
        if (debug) printf("| %s\n", trunc_text);
    else
        if (debug) printf("| %s\n", text);

    fflush(stdout);
    if (trunc_text) free(trunc_text);

    return SWIFT_SUCCESS;
}

/* 
 * This method blocks till current job is finished
 */
JNIEXPORT void JNICALL Java_cepstral_JNICepstral_waitTillDone
  (JNIEnv *, jobject) 
{
	if (swift_port_status(article_port, article_stream) >= SWIFT_STATUS_RUNNING)        
	{

        swift_port_wait(article_port, article_stream);
    }
}