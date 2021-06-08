/**
 * emulate encoder via OSC with Klangstrom Desktop Simulator
 */

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress mRemoteLocation;
float mMouseWheel = 0;
float mEncoderID = 0;

final String mAddrPattern = "/klang/sim";

void setup() {
    size(640, 480);
    oscP5 = new OscP5(this, 7000);
    mRemoteLocation = new NetAddress("224.0.0.1", 7001);
}

void draw() {
    background(64 + mEncoderID * 64);
}

void keyPressed() {
    switch (key) {
    case '0':
        mEncoderID = 0;
        break;
    case '1':
        mEncoderID = 1;
        break;
    case '2':
        mEncoderID = 2;
        break;
    }
}

void mouseWheel(MouseEvent event) {
    final float e = event.getCount();
    final float mPrevMouseWheel = mMouseWheel;
    mMouseWheel += e;
    OscMessage mMsg = new OscMessage(mAddrPattern);
    mMsg.add(0x09);             /* EVENT_ENCODER_ROTATE */
    mMsg.add(mEncoderID);       /* INDEX ( float ) */
    mMsg.add(mMouseWheel);      /* TICK ( float ) */
    mMsg.add(mPrevMouseWheel);  /* PREVIOUS_TICK ( float ) */
    oscP5.send(mMsg, mRemoteLocation);
    mMsg.print();
}

void mousePressed() {
    OscMessage mMsg = new OscMessage(mAddrPattern);
    mMsg.add(0x0A);             /* EVENT_ENCODER_BUTTON_PRESSED */
    mMsg.add(mEncoderID);       /* INDEX ( float ) */
    oscP5.send(mMsg, mRemoteLocation);
    mMsg.print();
}

void mouseReleased() {
    OscMessage mMsg = new OscMessage(mAddrPattern);
    mMsg.add(0x0B);             /* EVENT_ENCODER_BUTTON_RELEASED */
    mMsg.add(mEncoderID);       /* INDEX ( float ) */
    oscP5.send(mMsg, mRemoteLocation);
    mMsg.print();
}
