import javax.sound.sampled.*;
import java.io.File;
import java.io.IOException;
import java.net.URL;

public class AudioManager extends Manager {
    private Clip clip;

    public AudioManager(String source) {
        super(source);
        start();
    }

    private void start() {
        try {
            // Open an audio input stream.
            for (String file : audioFiles) {
                URL url = this.getClass().getClassLoader().getResource(file);
                assert url != null;
                // Get a sound clip resource.
                Clip clip = AudioSystem.getClip();
                super.audioClips.add(clip);
            }
        } catch (LineUnavailableException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void addFile(String name) throws Exception {
        String file = path + "\\" + name;
        File f = new File(file + extension);
        if (name.contains(extension)) f = new File(file);
        if (!f.getName().contains(extension)) throw new Exception("Invalid file");
        super.setAudioFile(f);
    }

    public void playClip(int finalI) {
        try {
            AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(new File(super.audioFiles.get(finalI)));
            clip = AudioSystem.getClip();
            clip.open(audioInputStream);
            clip.start();
        } catch (UnsupportedAudioFileException | IOException | LineUnavailableException e) {
            e.printStackTrace();
        }
    }

    public Clip getClip() {
        return clip;
    }

    public void setClip(Clip clip){
        this.clip = clip;
    }

    public void stop(int finalI) {
        clip.close();
        clip.stop();
    }
}