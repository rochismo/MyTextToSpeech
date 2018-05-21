import javax.swing.*;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static javax.swing.WindowConstants.EXIT_ON_CLOSE;

public class Gui {
    private AudioManager audioManager;
    private final JFrame frame;
    private int width;
    private int height;

    public Gui(JFrame frame, int width, int height, AudioManager audioManager){
        this.frame = frame;
        this.width = width;
        this.height = height;
        this.audioManager = audioManager;
        init();
    }

    private void init(){
        frame.setLayout(new java.awt.GridLayout(4,4));
        frame.setSize(width, height);
        frame.setDefaultCloseOperation(EXIT_ON_CLOSE);
        for (int i = 0; i < audioManager.getAudioFiles().size(); i++) {
            JButton b = new JButton(audioManager.getAudioNames().get(i).toUpperCase());
            int finalI = i;
            b.addActionListener(e -> {
                audioManager.setClip(audioManager.getAudioClips().get(finalI));
                if (audioManager.getClip().isActive()){
                    audioManager.stop(finalI);
                }
                audioManager.playClip(finalI);
            });
            frame.add(b);
        }
        frame.setVisible(true);
    }
}
