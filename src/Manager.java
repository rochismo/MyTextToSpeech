import javax.sound.sampled.Clip;
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public abstract class Manager {
    protected List<String> audioFiles;
    protected List<Clip> audioClips;
    private final File[] files;
    protected final String path;
    protected final String extension;
    protected List<String> audioNames;
    private JPanel jPanel;
    private List<Button> buttons;

    public Manager(String source) throws IOException {
        path = source;
        extension = ".wav";
        audioNames = new ArrayList<>();
        audioFiles = new ArrayList<>();
        audioClips = new ArrayList<>();
        files = new File(String.valueOf(path)).listFiles();
        init();
    }

    protected void init() {
        for (File file : files) {
            audioNames.add(file.getName().replaceAll(extension, ""));
            audioFiles.add(file.getAbsolutePath());
        }
    }

    @Override
    public String toString() {
        return Arrays.toString(audioFiles.toArray());
    }

    public List<Clip> getAudioClips() {
        return audioClips;
    }

    public List<String> getAudioNames(){
        return audioNames;
    }

    public String getPath(){
        return path;
    }

    public List<String> getAudioFiles() {
        return audioFiles;
    }

    public abstract void addFile(String name) throws Exception;

    protected void setAudioFile(File file) throws Exception {
        if (!file.exists()) throw new Exception("File doesn't exist");
        if (!audioFiles.contains(file.getAbsolutePath())){
            System.out.println("File won't be added because it exists");
            return;
        }
        audioFiles.add(file.getAbsolutePath());
    }
}
