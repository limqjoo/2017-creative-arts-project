# Like Wind, Like Water (2017) by Qian Joo, Lim

This repository contains the code for an art installation created as the final project for the BSc Digital Arts Computing at Goldsmiths University.


## About
*Like Wind, Like Water (2017)* instigates a reconsideration of the veracity of digitally-acquired information and knowledge in our everyday lives. The information is often presented through familiar and beguiling user interfaces of the digital platform.

The audience is presented with two different ways to interact with the work - via a desktop computer whose screen can be accessed and altered visually through a web application and via the web application on the audience’s mobile device. The audience of the desktop computer is prompted to search for information through a search engine. While desktop user tries to access information, users of the web app interfere with the desktop screen by altering it by using digital image processing techniques such as blurring and smudging. The act of warping the image by hand, and not through another device such as a mouse, becomes a metaphor for the distortion of truth. The power to shape what we perceive is reinterpreted in the form of the act of manipulating images. By allowing users to both disrupt and experience the disruption of the interface, it makes indistinct the positions of control and powerlessness one can have.

By disrupting the digital interface, Like Wind, Like Water intends to disturb our belief in the authenticity of the information through the familiar and beguiling user interfaces of the digital platform.
The project explores the interaction between two screens and one computer, providing a unique and immersive experience for users.


## Technical Overview

### Hardware

The initial exploration involved different combinations of devices, leading to the decision to use a desktop computer and a tablet for a more dynamic user experience.

### Live Streaming Desktop Screen

The project utilizes a VNC server (Real VNC Open) to capture the desktop screen, and a custom web client (adapted from noVnc) for real-time streaming to the tablet. Various attempts were made to overcome challenges related to UDP communication and data size limitations, eventually settling on the VNC server approach.

### Shaders for Real-time Image Manipulation

GLSL shaders are employed to achieve real-time image manipulation effects. The use of shaders allows for distortion effects to persist even when the displayed image changes. Challenges in adapting the code to different OpenGL versions were addressed, emphasizing the importance of using the lowest possible version for compatibility.

## System Architecture

The system consists of two main components:

Desktop Computer: Captures the desktop screen, processes the image using GLSL shaders, and communicates with the tablet.

Tablet (or Smartphone): Displays the live-streamed distorted image and allows user interaction.

The data flow between the two devices is depicted in the following diagram:


## Dependencies

The project relies on the following dependencies:

* ofxAwesomium: Implements a live webpage within the openFrameworks (oF) app.

* Socket-io: Facilitates communication between the NodeJs server and the web client.

* NodeJs's in-built dgram module: Enables data transmission from the server to the oF app.

* PngJs: Converts raw pixel data into PNG encoding for display on HTML5 canvas.

* ofxNetwork: Allows the oF app to receive and send data to the NodeJs server.


## Further Reading

[Original Project write-up](https://www.doc.gold.ac.uk/creativeprojects/index.php/2017/05/11/like-wind-like-water/)

[Work-in-progress blog posts](https://lqjoo.wordpress.com/category/cap/)


## License
This project is licensed under the MIT License.
