#include "testApp.h"

#define NUMROWS 200
#define NUMCOLS 200
#define SPACING 3

//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
    // enable depth->rgb image calibration
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();
    
    
    // setup mesh
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	for (int y = 0; y < NUMROWS; y++){
        for (int x = 0; x < NUMCOLS; x ++){
            int index = x + y * NUMROWS;
            mesh.addColor(ofColor(0));
			mesh.addVertex(ofPoint(x * SPACING, y * SPACING , 0));
		}
	}
    
//    // setup mesh indices
//	for (int y = 0; y < NUMROWS - 1; y++){
//        for (int x = 0; x < NUMCOLS; x++){
//            int index = x + y * NUMCOLS;
//            mesh.addIndex(index);
//            index = x + (y+1) * NUMCOLS;
//            mesh.addIndex(index);
//		}
//	}
    
    
    // http://dan.lecocq.us/wordpress/2009/12/25/triangle-strip-for-grids-a-construction/
    
    int counter = 0;
    int colSteps = NUMCOLS * 2;  
    int rowSteps = NUMROWS - 1;      
    
    for (int y = 0; y < rowSteps; y++) {
        for (int x = 0; x < colSteps; x++) {
            int index = x + y * colSteps;
            
            mesh.addIndex(counter);
            
            if(x == colSteps - 1){  // last col
                break;              // don't change counter (we double this one)
            }
            else{
                if(index % 2 == 0){ // even indices
                    counter += NUMCOLS; // move counter to next row
                }
                else{
                    if(y % 2 == 0){ // uneven indices, even rows
                        counter -= NUMCOLS - 1; // move back to previous row, and move one to the left
                    }
                    else{           // uneven indices, uneven rows
                        counter -= NUMCOLS + 1; // move back to previous row and move one to the right
                    }
                }
            }
        }
    }

}

//--------------------------------------------------------------
void testApp::update() {	
	kinect.update();
    
	if(kinect.isFrameNew()) {
        for (int y = 0; y < NUMROWS; y++){
            for (int x = 0; x < NUMCOLS; x ++){
                int kinectX = ofMap(x, 0, NUMCOLS, 0, kinect.getWidth());
                int kinectY = ofMap(y, 0, NUMROWS, 0, kinect.getHeight());
                
                int index = x + y * NUMCOLS;
                float distance = kinect.getDistanceAt(kinectX, kinectY);
                
                if(distance > 100 && distance < 1500){
                    ofPoint vertex = ofPoint(x * SPACING, y * SPACING, distance - 1000);
                    ofColor color = kinect.getColorAt(kinectX, kinectY);
                    
                    mesh.setVertex(index, vertex);
                    mesh.setColor(index, color);
                }
                else{
                    mesh.setColor(index, ofColor(0,0));
                }
            }
        }

	}
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackgroundGradient(ofColor(50), ofColor(0));
    // draw from the live kinect
//    ofSetColor(255);
//    kinect.drawDepth(10, 10, 400, 300);
//    kinect.draw(420, 10, 400, 300);

    
    glEnable(GL_DEPTH_TEST);
    static ofEasyCam cam;
    cam.begin();
    ofScale(1, -1);
    ofTranslate(-320, -240);
    
    ofSetColor(255);
    mesh.drawFaces();

//    glPointSize(10);
//    mesh.drawVertices();
    
    cam.end();
    
    ofSetColor(0);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
}

//--------------------------------------------------------------
void testApp::exit() {
	kinect.close();
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}
