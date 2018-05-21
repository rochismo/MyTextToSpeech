package Sample;
import cepstral.JNICepstral;
public class Demo {
	public static void main (String args[]) {	
		
		JNICepstral.initializeEngine();
			
		JNICepstral.speaktext("Cepstral provides speech technologies and services");
		
		JNICepstral.setVoice("David");
		System.out.println("Text will now be spoken with Cepstral David voice");		
		JNICepstral.shutdownEngine();
		
		JNICepstral.initializeEngine();		
		System.out.println("You will experience how fast I can read. " +  
				"Seasoned users can listen to audio upto 400 wpm");
		JNICepstral.setSpeakingRate(60);
		JNICepstral.speaktext("Speak rate is 60 words per minute");
		JNICepstral.shutdownEngine();
		
		JNICepstral.initializeEngine();		
		JNICepstral.setSpeakingRate(250);
		JNICepstral.speaktext("Speak rate is 250 words per minute."); 
						
		JNICepstral.shutdownEngine();
		
		JNICepstral.initializeEngine();			
		JNICepstral.speaktext("We now begin to test the volume levels");		
		JNICepstral.speaktext("This is how loud I can sound at volume of 200");
		JNICepstral.setVol(200);		
		JNICepstral.speaktext("And this is how loud I sound at volume of 40");
		JNICepstral.setVol(40);
		JNICepstral.waitEngine();
		JNICepstral.setVol(100); /* Setting it back to default */
		JNICepstral.shutdownEngine();		
		

	}

}
