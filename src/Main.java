import javax.swing.*;

public class Main {

    private static final String PATH = "C:\\Users\\Benjamin\\IdeaProjects\\casa\\MyTextToSpeech\\src\\audio";
    private static Gui gui;

    private static void init(){
        AudioManager m = new AudioManager(PATH);
        int width = m.getAudioFiles().size() * 10;
        int height = m.getAudioFiles().size() * 10;
        Gui gui = new Gui(new JFrame("Test"), (width * 2) , (height * 2), m);
    }

    public static void main(String[] args) throws Exception {
        init();
    }
}
