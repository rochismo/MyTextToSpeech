package cepstral;

/**
 * To communicate with Cepstral DLL
 * 
 *
 */
public class JNICepstral {

// TODO: this file is supposed to be in the cepstral.jni package.
// changing the package name will require recompiling the DLLs
// perhaps we could better design the directory structure
// and eliminate dependencies such as that existing in the DLL
	
  private static JNICepstral voice;
  /*Krishna: Following flag isn't of any use
   * public static boolean cepstralPause = false;
  static public boolean t = true;*/

  private native boolean initialize();

  private native boolean shutdown();

  private native int cancel(int portNo);

  private native boolean pauseArticle();

  private native boolean resumeArticle();

  private native int setVolume(int v);

  private native int setSpeed(int s);

  private native boolean sayArticle(String s);

  //XXX: We don't use this method in hearsay and so 
  // leave it exposed
  //public native boolean sayText(String s);

  private native boolean isTextPlaying(int portNo);
    
  private native boolean stopText();

  private native boolean changeVoice(String s);

  private native double setPitch(double p);
  
  private native void waitTillDone();
  
  //Added following method to fetch voice list from engine.
  

  /**
   * Loads the dependent DLL's required for Cepstral swift.dll JNICeptral.dll
   * 
   */
  static {	
	System.loadLibrary("JNICepstralWrapper");
	voice = new JNICepstral();
  }

  /**
   * To Initialize the Engine
   */
  public static void initializeEngine() {
    voice.initialize();
  }

  /**
   * To Cancel the current Text in Engine
   */
  public static void cancelEngine() {
    voice.cancel(1);
  }
  /**
   * To check whether Engine is Speaking or NOT
   * 
   * @return true is playing
   * @return false is NOT playing
   */

  public static boolean getEngineStatus() {
    return (voice.isTextPlaying(1)); // parameter 1 indicates its an
    // article port
  }

  /**
   * Set the Voice for Cepstral Engine
   * 
   * @param s The voice name to which the engine is set.
   * String : Name of the Voice
   */
  public static boolean setVoice(String s) {
	return voice.changeVoice(s);
  }


  /**
   * Closes the Cepstral Engine
   */
  public static void shutdownEngine() {
    voice.shutdown();
  }

  /**
   * To speak the given input string
   * 
   * @param string to be spoken
   */
  public static void speaktext(String textToSpeak) {	  
    voice.sayArticle(textToSpeak);
  }

  /**
   * Set the Volume for Cepstral Engine
   * 
   * @param integer
   *            value v
   * 
   */
  public static void setVol(int v) {
    voice.setVolume(v);
  }

  /**
   * Set the SpeakingRate for Cepstral Engine
   * 
   * @param integer
   *            Set the rate to value v
   * 
   */
  public static void setSpeakingRate(int v) {
	// Safety check, higher values sometime crash cepstral
	// Empirically determined value of 500 is used.
	if (v > 500)
		v = 500;
    voice.setSpeed(v);

  }

  
  /**
   * Change the pitch for Cepstral Engine
   * 
   * @param integer
   *            value v
   * 
   */
  public static void changePitch(double v) {
    // Default pitch value is 1.0
	// Larger values can crash the engine
	if (v > 10)
		v = 10;
    voice.setPitch(v);
  }

  /**
   * Function used to shutdown Cepstral Engine
   * 
   */
  public static void deallocateVoice() {
    voice.shutdown();
  }

  /**
   * Used to pause the current speech by the Cepstral Engine
   * 
   */
  public static void pause() {    
	voice.pauseArticle();
  }

  /**
   * Resume the speech which was paused earlier for Cepstral Engine
   * 
   */
  public static void resume() {
    voice.resumeArticle();
  }

  
  /**
   * This method is a wrapper around the native method to stop the 
   * current article being read and purge the stream.
   */
  public static void stopEngine() {
	  voice.stopText();
  }
  
  /**
   * This wrapper method blocks till the current job finishes. 
   * This is a useful utility that other java methods can use to 
   * block.
   */
  public static void waitEngine() {
	  voice.waitTillDone();
  }
}
