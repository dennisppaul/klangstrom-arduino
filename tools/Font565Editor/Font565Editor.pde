import netP5.*; 
import oscP5.*; 
import controlP5.*; 
import themidibus.*; 
import processing.sound.*; 

static final String FOLDER_FILE_DELIMITER = "/"; // @todo(does this also work on windows?)

static final Vec2i DEFAULT_FONT_DIMENSIONS = new Vec2i(5, 8);

String mAbsolutFolderPath;

KLSTChar mChar;

int mCurrentCharIndex = 0;

KLSTFont mFont;

boolean mRecordCharacters = false;

String mRecordedString = "";

void settings() {
    size(1024, 768);
}

void setup() {
    noStroke();
    fill(0);
    textFont(createFont("Roboto Mono", 10));
    /* load font from disk or use default font */
    selectInput("Select a font file:", "fileSelected");
    while (mFont == null) {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

void folderSelected(File selection) {
    if (selection != null) {
        println("+++ folder to save font file to: " + selection.getAbsolutePath());
        mFont = new KLSTFont(DEFAULT_FONT_DIMENSIONS.x, DEFAULT_FONT_DIMENSIONS.y);
        mAbsolutFolderPath = selection.getAbsolutePath();
        init_character();
    } else {
        println("+++ exiting application");
        exit();
    }
}

void fileSelected(File selection) {
    if (selection == null) {
        println("+++ creating empty font");
        selectFolder("Select a folder to save font file to:", "folderSelected");
    } else {
        mFont = KLSTFontEmitter.parse_font(this, selection.getAbsolutePath());
        String mFilePath = selection.getAbsolutePath();
        mAbsolutFolderPath = mFilePath.substring(0, mFilePath.lastIndexOf(FOLDER_FILE_DELIMITER));
        init_character();
    }
}

void draw() {
    background(255);
    fill(0);
    String mOSDStr = get_OSD();
    text(mOSDStr, 50, 35);
    if (mChar != null) {
        mChar.position.set(50, 50);
        mChar.draw(g, true, false);
        mChar.hit(mouseX, mouseY);
        mFont.position.set(mChar.position);
        mFont.position.x += mChar.scale * mFont.char_width;
        mFont.position.x += mChar.scale;
        mFont.position.x += mFont.padding_x * mFont.scale;
        mFont.position.y += mFont.padding_y * mFont.scale;
    }
    mFont.draw(g, mCurrentCharIndex);
    noStroke();
    fill(255, 127, 0);
    mFont.draw_text(g, mRecordedString, 50, height - 100, mFont.scale);
    if (mRecordCharacters) {
        if ((frameCount / 30) % 2 == 0) {
            noStroke();
            fill(0);
            rect(50 - mFont.char_width * mFont.scale - 3,
                 height - 100,
                 3 * mFont.scale,
                 mFont.char_height * mFont.scale);
        }
    }
}

void keyPressed() {
    switch (keyCode) {
        case CONTROL:
            mRecordCharacters = !mRecordCharacters;
            break;
        case BACKSPACE:
            if (mRecordedString.length() > 0) {
                mRecordedString = mRecordedString.substring(0, mRecordedString.length() - 1);
            }
            break;
    }
    if (mRecordCharacters) {
        int i = mFont.get_char_index(key);
        if (i >= 0 && i < KLSTFont.NUM_CHARS) {
            mRecordedString += key;
        }
        return;
    }
    switch (keyCode) {
        case LEFT:
            mCurrentCharIndex--;
            mCurrentCharIndex += KLSTFont.NUM_CHARS;
            mCurrentCharIndex %= KLSTFont.NUM_CHARS;
            mChar = new KLSTChar(mFont, mCurrentCharIndex);
            break;
        case RIGHT:
            mCurrentCharIndex++;
            mCurrentCharIndex %= KLSTFont.NUM_CHARS;
            mChar = new KLSTChar(mFont, mCurrentCharIndex);
            break;
        case UP:
            if (mCurrentCharIndex >= mFont.wrap) {
                mCurrentCharIndex -= mFont.wrap;
                mChar = new KLSTChar(mFont, mCurrentCharIndex);
            }
            break;
        case DOWN:
            if (mCurrentCharIndex < KLSTFont.NUM_CHARS - mFont.wrap) {
                mCurrentCharIndex += mFont.wrap;
                mChar = new KLSTChar(mFont, mCurrentCharIndex);
            }
            break;
        case java.awt.event.KeyEvent.VK_ENTER:
            KLSTFontEmitter.save_font(this, mFont, mAbsolutFolderPath);
            break;
    }
    switch (key) {
        case '+':
            mFont.scale += 1.0f;
            break;
        case '-':
            mFont.scale -= 1.0f;
            break;
        case '0':
            mFont.scale = 1.0f;
            break;
        case 's':
            mChar.save_char();
            break;
        case 'o':
            selectInput("Select a font file:", "fileSelected");
            break;
        case ',':
            mFont.baseline++;
            break;
        case '.':
            mFont.baseline--;
            break;
        case 'n':
            mFont.x_height++;
            break;
        case 'm':
            mFont.x_height--;
            break;
    }
}

void mousePressed() {
    if (mChar != null && mChar.hit != null) {
        mChar.swap_pixel(mChar.hit.x, mChar.hit.y);
    }
}

void init_character() {
    mCurrentCharIndex = mFont.get_char_index('a');
    mChar = new KLSTChar(mFont, mCurrentCharIndex);
}

String get_OSD() {
    return "'" + mFont.get_index_char(mCurrentCharIndex) + "'" + " :: " + nf(mCurrentCharIndex, 3) + " (" + nf(
    mCurrentCharIndex + KLSTFont.ASCII_CHAR_OFFSET, 3) + ")" + (mChar.is_changed ? " *" : "");
}

static class KLSTFontEmitter {
    /* example font name: KlangstromDisplayFont_7x10.cpp */
    
static final String FONT_FILE_NAME_PREFIX = "KlangstromDisplay";
    
static final String FONT_NAME_PREFIX = "Font_";
    
static final String FONT_FILE_NAME_SUFFIX = ".cpp";
    
static final String FONT_NAME_DIMENSION_DELIMITER = "x";
    
static final String HEX_NUMBER_PREFIX = "0x";
    
static final String HEADER_COPYRIGHT_NOTICE = "/*\n" + " * Klangstrom\n" + " *\n" + " * This file is "
                                                          + "part of the *wellen* library " + "(https://github" + ".com/dennisppaul/wellen).\n" + " * Copyright (c) 2022 " + "Dennis P Paul.\n" + " *\n" + " * This library is free " + "software: you can redistribute" + " it and/or " + "modify\n" + " * it under the terms of the GNU General " + "Public " + "License as published by\n" + " * the Free " + "Software Foundation, version 3.\n" + " *\n" + " * This" + " library is distributed in the hope that it " + "will " + "be useful, but\n" + " * WITHOUT ANY WARRANTY; without " + "even the implied " + "warranty of\n" + " * " + "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE" + ". See the GNU\n" + " * General Public License for more details.\n" + " *\n" + " * You should have received a copy of the GNU General " + "Public License\n" + " * along with this program. If not, see <http://www" + ".gnu.org/licenses/>.\n" + " */\n";
    
static final String HEADER_INCLUDE_DIRECTIVE = "#include \"KlangstromDisplayFont.h\"\n";
    
static final String FONT_DATA_HEADER_PREFIX = "static const uint16_t ";
    
static final String FONT_DATA_HEADER_SUFFIX = "[] = {";
    
static final String FONT_DATA_ARRAY_CLOSE = "};";
    
static final String FONT_DATA_ARRAY_DELIMITER = ", ";
    
static final String FONT_DATA_ARRAY_PREFIX = "_data";
    
static final String FONT_DATA_ARRAY_TAB = "    ";
    
static final String FONT_CLASS_NAME = "KlangstromDisplayFont";
    
static KLSTFont parse_font(PApplet p, String pFontPath) {
        System.out.println("+++ parsing font   : " + pFontPath);
        String mFontFileName = get_font_filename_from_path(pFontPath);
        String mFontName = get_font_name_from_file(mFontFileName);
        Vec2i mFontDimension = get_font_dimensions_from_name(mFontName);
        String[] mContent = p.loadStrings(pFontPath);
        int[] mFontData = get_font_data(mContent);
        return new KLSTFont(mFontDimension.x, mFontDimension.y, mFontData);
    }
    
static void save_font(PApplet p, KLSTFont pFont, String pAbsolutFontFilePath) {
        String mFontName = FONT_NAME_PREFIX + pFont.char_width + FONT_NAME_DIMENSION_DELIMITER + pFont.char_height;
        String mFontFileName =
        pAbsolutFontFilePath + FOLDER_FILE_DELIMITER + FONT_FILE_NAME_PREFIX + mFontName + FONT_FILE_NAME_SUFFIX;
        System.out.println("+++ saving font    : " + mFontFileName);
        ArrayList<String> mFontFileContent = new ArrayList();
        mFontFileContent.add(HEADER_COPYRIGHT_NOTICE);
        mFontFileContent.add(HEADER_INCLUDE_DIRECTIVE);
        mFontFileContent.add(
        FONT_DATA_HEADER_PREFIX + mFontName + FONT_DATA_ARRAY_PREFIX + FONT_DATA_HEADER_SUFFIX);
        add_font_data(pFont, mFontFileContent);
        mFontFileContent.add(FONT_DATA_ARRAY_CLOSE);
        mFontFileContent.add(
        FONT_CLASS_NAME + " " + mFontName + " = {" + pFont.char_width + FONT_DATA_ARRAY_DELIMITER + pFont.char_height + FONT_DATA_ARRAY_DELIMITER + mFontName + FONT_DATA_ARRAY_PREFIX + FONT_DATA_ARRAY_CLOSE);
        String[] mFontFileContentArray = new String[mFontFileContent.size()];
        mFontFileContent.toArray(mFontFileContentArray);
        p.saveStrings(mFontFileName, mFontFileContentArray);
    }
    
static void add_font_data(KLSTFont pFont, ArrayList<String> pFontFileContent) {
        StringBuilder mContent = new StringBuilder();
        for (int i = 0; i < pFont.data.length; i++) {
            if (i % pFont.char_height == 0) {
                mContent.append(FONT_DATA_ARRAY_TAB);
            }
            mContent.append(String.format("0x%04X", pFont.data[i]));
            mContent.append(FONT_DATA_ARRAY_DELIMITER);
            if (i % pFont.char_height == pFont.char_height - 1) {
                int mCharIndex = i / pFont.char_height;
                mContent.append("/*");
                mContent.append(" ");
                if (KLSTFont.ASCII_C_SET[mCharIndex] == ' ') {
                    mContent.append("SPACE");
                } else {
                    mContent.append(KLSTFont.ASCII_C_SET[mCharIndex]);
                }
                mContent.append(" ");
                mContent.append("*/");
                if (i < pFont.data.length - 1) {
                    mContent.append("\n");
                }
            }
        }
        pFontFileContent.add(mContent.toString());
    }
    
static int[] get_font_data(String[] pContent) {
        ArrayList<String> mContent = new ArrayList();
        boolean mRecord = false;
        for (String s : pContent) {
            if (s.contains("};")) {
                break;
            }
            if (mRecord) {
                mContent.add(s);
            }
            if (s.contains("= {")) {
                mRecord = true;
            }
        }
        System.out.println("+++ found chars    : " + mContent.size());
        ArrayList<String> mDataStr = new ArrayList();
        for (String mRawDataLine : mContent) {
            mRawDataLine = mRawDataLine.trim();
            String[] mDataEntry = mRawDataLine.split(",");
            for (String mDatum : mDataEntry) {
                mDatum = mDatum.trim();
                if (mDatum.startsWith(HEX_NUMBER_PREFIX)) {
                    mDatum = mDatum.replace(HEX_NUMBER_PREFIX, "");
                    mDataStr.add(mDatum);
                }
            }
        }
        int[] mDataArray = new int[mDataStr.size()];
        for (int i = 0; i < mDataStr.size(); i++) {
            String mDatum = mDataStr.get(i);
            mDataArray[i] = Integer.valueOf(mDatum, 16);
        }
        return mDataArray;
    }
    
static Vec2i get_font_dimensions_from_name(String pFontName) {
        pFontName = pFontName.substring(pFontName.lastIndexOf('_') + 1); // remove name
        String[] mDimensionStr = pFontName.split(FONT_NAME_DIMENSION_DELIMITER); // split dimensions
        Vec2i mDimension = new Vec2i(Integer.parseInt(mDimensionStr[0]), Integer.parseInt(mDimensionStr[1]));
        System.out.println("+++ font dimensions: " + mDimension.x + ", " + mDimension.y);
        return mDimension;
    }
    
static String get_font_filename_from_path(String pFontPath) {
        String[] s = pFontPath.split(FOLDER_FILE_DELIMITER);
        String mFontName = s[s.length - 1];
        System.out.println("+++ font file name : " + mFontName);
        return mFontName;
    }
    
static String get_font_name_from_file(String pFontName) {
        pFontName = pFontName.substring(0, pFontName.lastIndexOf('.')); // remove extension
        pFontName = pFontName.replace(FONT_FILE_NAME_PREFIX, "");
        System.out.println("+++ font name      : " + pFontName);
        return pFontName;
    }
}

static class KLSTChar {
    
static final float TOTAL_FONT_HEIGHT = 500.0f;
    
static int COLOR_FOREGROUND = 0;
    
static int COLOR_BACKGROUND_DARK = 255;
    
static int COLOR_GRID = 255;
    
static int COLOR_GRID_INVERT = 0;
    
static int COLOR_BACKGROUND_LIGHT = 240;
    
static int COLOR_HIGHLIGHT = -1090486273; // color()
    final boolean[][] data;
    Vec2i hit;
    final int index;
    boolean is_changed;
    final KLSTFont mFont;
    final PVector position;
    float scale;
    
KLSTChar(KLSTFont pFont, int pIndex) {
        mFont = pFont;
        index = pIndex;
        scale = TOTAL_FONT_HEIGHT / mFont.char_height;
        position = new PVector();
        data = convert_to_boolean(mFont.get_char_data(index));
        is_changed = false;
    }
    
boolean[][] convert_to_boolean(int[] pData) {
        boolean[][] mData = new boolean[mFont.char_width][mFont.char_height];
        for (int i = 0; i < pData.length; i++) {
            for (int j = 0; j < mFont.char_width; j++) {
                final int n = 15 - j;
                final boolean mIsSet = is_bit_set(pData[i], n);
                mData[j][i] = mIsSet;
            }
        }
        return mData;
    }
    
void save_char() {
        System.out.println("+++ saved character");
        int mDataIndex = index * mFont.char_height;
        for (int i = 0; i < mFont.char_height; i++) {
            int c = 0;
            for (int j = 0; j < mFont.char_width; j++) {
                final int n = 15 - j;
                c = set_bit(c, n, data[j][i]);
            }
            mFont.data[mDataIndex + i] = c;
        }
        is_changed = false;
    }
    
void draw(PGraphics g, boolean pGrid, boolean pInvert) {
        g.pushMatrix();
        g.translate(position.x, position.y);
        g.noStroke();
        for (int x = 0; x < data.length; x++) {
            for (int y = 0; y < data[x].length; y++) {
                int mFillBackground;
                if (data[x][y]) {
                    mFillBackground = pInvert ? COLOR_BACKGROUND_DARK : COLOR_FOREGROUND;
                } else {
                    mFillBackground = pInvert ? COLOR_FOREGROUND : (pGrid ? COLOR_BACKGROUND_LIGHT :
                                                                    COLOR_BACKGROUND_DARK);
                }
                g.stroke(mFillBackground);
                g.fill(mFillBackground);
                final float mX = x * scale;
                final float mY = y * scale;
                g.square(mX, mY, scale);
            }
        }
        if (hit != null) {
            final float x = hit.x * scale;
            final float y = hit.y * scale;
            g.fill(COLOR_HIGHLIGHT);
            g.square(x, y, scale);
        }
        if (pGrid) {
            g.noFill();
            g.stroke(pInvert ? COLOR_GRID_INVERT : COLOR_GRID);
            for (int x = 0; x < data.length; x++) {
                for (int y = 0; y < data[x].length; y++) {
                    final float mX = x * scale;
                    final float mY = y * scale;
                    g.square(mX, mY, scale);
                }
            }
            draw_baseline(g);
            draw_x_height(g);
        }
        g.popMatrix();
    }
    
void swap_pixel(int x, int y) {
        is_changed = true;
        data[x][y] = !data[x][y];
    }
    
void hit(float x, float y) {
        float mX = x - position.x;
        float mY = y - position.y;
        if (mX < 0 || mY < 0) {
            hit = null;
            return;
        }
        int mIndexX = (int) (mX / scale);
        int mIndexY = (int) (mY / scale);
        if (mIndexX >= mFont.char_width || mIndexX < 0 || mIndexY >= mFont.char_height || mIndexY < 0) {
            hit = null;
            return;
        }
        hit = new Vec2i(mIndexX, mIndexY);
    }
    
boolean is_bit_set(int pValue, int pBitSet) {
        return ((pValue & (1L << pBitSet)) != 0);
    }
    
int set_bit(int pValue, int pBitSet, boolean pBitState) {
        if (pBitState) {
            pValue |= (1 << pBitSet);
        } else {
            pValue &= ~(1 << pBitSet);
        }
        return pValue;
    }
    
void draw_baseline(PGraphics g) {
        g.stroke(KLSTFont.COLOR_BASELINE);
        final float mX = data.length * scale;
        final float mY = mFont.baseline * scale;
        g.line(0, mY, mX, mY);
    }
    
void draw_x_height(PGraphics g) {
        g.stroke(KLSTFont.COLOR_BASELINE);
        final float mX = data.length * scale;
        final float mY = mFont.x_height * scale;
        g.line(0, mY, mX, mY);
    }
}

static class Vec2i {
    final int x;
    final int y;
    Vec2i(int pX, int pY) {
        x = pX;
        y = pY;
    }
}

static class KLSTFont {
    
static final char[] ASCII_C_SET = {' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-',
                                              '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';',
                                              '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                                              'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                                              'X', 'Y', 'Z', '\'', '\\', '\'', '^', '_', '`', 'a', 'b', 'c', 'd',
                                              'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                                              's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'};
    
static final int ASCII_CHAR_OFFSET = ASCII_C_SET[0];
    
static final int NUM_CHARS = ASCII_C_SET.length;
    
static final int COLOR_BACKGROUND = 0;
    
static final int COLOR_BASELINE = 91;
    
int baseline;
    
final int char_height;
    
final int char_width;
    
final int[] data;
    
final int padding_x;
    
final int padding_y;
    
final PVector position;
    
float scale = 1.0f;
    
final int wrap = 10;
    
float x_height;
    
KLSTFont(int pCharWidth, int pCharHeight) {
        this(pCharWidth, pCharHeight, new int[ASCII_C_SET.length * pCharHeight]);
    }
    
KLSTFont(int pCharWidth, int pCharHeight, int[] pFata) {
        char_width = pCharWidth;
        char_height = pCharHeight;
        baseline = char_height - 3;
        x_height = 2;
        padding_x = char_width / 2;
        padding_y = char_width / 2;
        data = pFata;
        position = new PVector();
        if (data.length / char_height != ASCII_C_SET.length) {
            System.err.println("+++ warning / number of characters, data size and character height do not match.");
        }
    }
    
int[] get_char_data(int pIndex) {
        final int[] mCharData = new int[char_height];
        final int mCharDataOffset = pIndex * char_height;
        for (int i = 0; i < mCharData.length; i++) {
            mCharData[i] = data[i + mCharDataOffset];
        }
        return mCharData;
    }
    
int get_char_index(char c) {
        return c - ASCII_CHAR_OFFSET;
    }
    
char get_index_char(int c) {
        return (char) (c + ASCII_CHAR_OFFSET);
    }
    
void draw(PGraphics g, int pCurrentCharIndex) {
        g.pushMatrix();
        g.translate(position.x, position.y);
        draw_current_grid_cell(g, pCurrentCharIndex);
        draw_char_set(g, pCurrentCharIndex);
        draw_char_set_grid(g);
        g.popMatrix();
    }
    
void draw_current_grid_cell(PGraphics g, int pCurrentCharIndex) {
        g.noStroke();
        g.fill(COLOR_BACKGROUND);
        draw_grid_cell(g, pCurrentCharIndex);
    }
    
void draw_char_set_grid(PGraphics g) {
        g.noFill();
        g.stroke(COLOR_BACKGROUND);
        for (int i = 0; i < NUM_CHARS; i++) {
            draw_grid_cell(g, i);
        }
    }
    
void draw_grid_cell(PGraphics g, int i) {
        PVector p = new PVector();
        get_char_position_from_index(i, p);
        p.x -= scale * (padding_x * 2) * 0.5f;
        p.y -= scale * (padding_y * 2) * 0.5f;
        g.rect(p.x, p.y, scale * (char_width + padding_x * 2), scale * (char_height + padding_y * 2));
    }
    
void draw_char_set(PGraphics g, int pCurrentCharIndex) {
        g.noStroke();
        g.fill(COLOR_BACKGROUND);
        for (int i = 0; i < NUM_CHARS; i++) {
            KLSTChar c = new KLSTChar(this, i);
            c.scale = scale;
            get_char_position_from_index(i, c.position);
            c.draw(g, false, i == pCurrentCharIndex);
        }
    }
    
void draw_text(PGraphics g, String pText, float x, float y, float pScale) {
        g.pushMatrix();
        g.translate(x, y);
        for (int i = 0; i < pText.length(); i++) {
            char c = pText.charAt(i);
            int j = get_char_index(c);
            if (j > 0 && j < NUM_CHARS) {
                KLSTChar mChar = new KLSTChar(this, j);
                mChar.scale = pScale;
                mChar.position.x = char_width * pScale * i;
                mChar.draw(g, false, false);
            }
        }
        g.popMatrix();
    }
    
void get_char_position_from_index(int i, PVector p) {
        p.x = (i % wrap) * scale * (char_width + padding_x * 2);
        p.y = (float) (i / wrap) * scale * (char_height + padding_y * 2);
    }
}
