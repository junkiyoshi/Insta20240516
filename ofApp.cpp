#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->base_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh.clear();

	int radius = 0;
	ofColor color;

	for (int i = 0; i < 60; i++) {

		int deg_base = ofRandom(360);
		int deg = deg_base;

		vector<glm::vec2> log;
		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		log.push_back(location);
		auto next = glm::vec2((radius - 10) * cos(deg * DEG_TO_RAD), (radius - 10) * sin(deg * DEG_TO_RAD));

		auto distance = location - next;
		distance *= 3;

		auto future = location + distance * 30;
		auto random_deg = ofRandom(360);
		future += glm::vec2(120 * cos(random_deg * DEG_TO_RAD), 120 * sin(random_deg * DEG_TO_RAD));
		auto future_distance = future - location;

		this->log_list.push_back(log);
		this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance) * ofRandom(0.25, 1.6));
		color.setHsb(ofMap(deg_base, 0, 345, 0, 255), 200, 255);
		this->color_list.push_back(ofColor(color, 16));
	}

	for (int i = this->log_list.size() - 1; i > -1; i--) {

		auto location = this->log_list[i][this->log_list[i].size() - 1];
		auto feature = location + glm::normalize(this->velocity_list[i]) * 80;
		auto deg = ofMap(ofNoise(glm::vec3(location * 0.003, ofGetFrameNum() * 0.01)), 0, 1, -360, 360);
		feature += glm::vec2(60 * cos(deg * DEG_TO_RAD), 60 * sin(deg * DEG_TO_RAD));
		location += glm::normalize(feature - location) * glm::length(this->velocity_list[i]);

		if (glm::distance(glm::vec2(), this->log_list[i][this->log_list[i].size() - 1]) > 1300) {

			this->log_list.erase(this->log_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);

			continue;
		}

		this->log_list[i].push_back(location);
		this->velocity_list[i] *= 1.01;
	}

	for (int i = 0; i < this->log_list.size(); i++) {

		for (int k = 0; k < this->log_list[i].size(); k++) {

			this->mesh.addVertex(glm::vec3(this->log_list[i][k], 0));
			this->mesh.addColor(this->color_list[i]);

			if (k > 0) {

				this->mesh.addIndex(this->mesh.getNumVertices() - 1); this->mesh.addIndex(this->mesh.getNumVertices() - 2);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	this->mesh.drawWireframe();

	ofFill();
	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		ofSetColor(this->mesh.getColor(i));
	}

	/*
	int start = 150;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}