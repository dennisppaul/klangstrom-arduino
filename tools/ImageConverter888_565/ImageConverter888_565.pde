import netP5.*; 
import oscP5.*; 
import controlP5.*; 
import themidibus.*; 
import processing.sound.*; 


final static int WRAP_LINES_ELEM = 13;

static final String FOLDER_FILE_DELIMITER = "/";

String mAbsolutFontFilePath;

final ArrayList<String> mImagePixels = new ArrayList();

void settings() {
    size(1024, 768);
}

void setup() {
    background(255);
    selectInput("Select an image file to convert:", "fileSelected");
    while (mAbsolutFontFilePath == null) {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    String mFolderName = mAbsolutFontFilePath.substring(0, mAbsolutFontFilePath.lastIndexOf(FOLDER_FILE_DELIMITER));
    String mImageFileName = mAbsolutFontFilePath.substring(
    mAbsolutFontFilePath.lastIndexOf(FOLDER_FILE_DELIMITER) + 1);
    String mImageName = mImageFileName.substring(0, mImageFileName.lastIndexOf('.'));
    PImage p = loadImage(mFolderName + FOLDER_FILE_DELIMITER + mImageFileName);
    image(p, width / 2.0f - p.width / 2.0f, height / 2.0f - p.height / 2.0f);
    writeHeader(p, mImageName);
    writeContent(p);
    writeFooter();
    writeFile(mImageName, mFolderName, p);
    exit();
}

void fileSelected(File selection) {
    if (selection != null) {
        mAbsolutFontFilePath = selection.getAbsolutePath();
    }
}

void writeFile(String pImageName, String pFolderName, PImage p) {
    String mFileName = pImageName;
    mFileName += "_" + p.width + "x" + p.height + ".h";
    String[] mContent = new String[mImagePixels.size()];
    mImagePixels.toArray(mContent);
    saveStrings(pFolderName + FOLDER_FILE_DELIMITER + mFileName, mContent);
}

void writeHeader(PImage p, String pImageName) {
    final String mWidthStr = pImageName + "_WIDTH";
    final String mHeightStr = pImageName + "_HEIGHT";
    final String mArrayStr = "data_" + pImageName;
    mImagePixels.add("#define " + mWidthStr + "   " + p.width);
    mImagePixels.add("#define " + mHeightStr + "  " + p.height);
    mImagePixels.add("");
    mImagePixels.add("const PROGMEM uint16_t " + mArrayStr + "[" + mWidthStr + " * " + mHeightStr + "] = {");
}

void writeFooter() {
    mImagePixels.add("};");
}

int convert_RGB_888i_565i(int r, int g, int b) {
    int c;
    c = (r * 31 / 255) << 11;
    c |= ((g * 63 / 255) << 5);
    c |= ((b * 31 / 255));
    return c;
}

void writeContent(PImage p) {
    p.loadPixels();
    StringBuilder s = new StringBuilder();
    for (int i = 0; i < p.pixels.length; i++) {
        int q = p.pixels[i];
        int c = convert_RGB_888i_565i((int) red(q), (int) green(q), (int) blue(q));
        c = swap_endians(c);
        s.append(String.format("0x%04X", c));
        if (i < p.pixels.length - 1) {
            s.append(", ");
        }
        if (i % WRAP_LINES_ELEM == WRAP_LINES_ELEM - 1) {
            mImagePixels.add(s.toString());
            println(s);
            s = new StringBuilder();
        }
    }
    if (s.length() > 0) {
        mImagePixels.add(s.toString());
    }
}

static int swap_endians(int value) {
    int leftmost_byte = (value & 0x000000FF) >> 0;
    leftmost_byte <<= 8;
    int rightmost_byte = (value & 0x0000FF00) >> 8;
    rightmost_byte <<= 0;
    return (leftmost_byte | rightmost_byte);
}
