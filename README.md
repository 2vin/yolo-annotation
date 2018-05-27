# annotation-yolo
YOLO-style annotation tool for preparing dataset of a new object from videos directly

This program can be used to prepare/ annotate new class in YOLO ![Demo](https://github.com/pjreddie/darknet)     

# Getting started    
1. Record videos of the object to be annotated. For instance, we will annotate oranges fromt the sample video.    
2. Make an empty directory to save annotation results. For instance, `dataset` in this case.    
3. Compile the program using following command:    
    `g++ annotate.cpp -o main \`pkg-config --cflags --libs opencv\``     
4. After successfully compiling the program, run the following command to start annotation process.    
    `./main video_path output_folder`     
    In this case, `./main ./oranges.mp4 ./dataset/    
5. This will open a frame window where you can select the objects using mouse.    
6. Once all the objects of defined class are selected in the image, double press 's' to confirm the selections to be saved.    
7. This will generate the YOLO-style annotation files for the object from the video.   

# Program in action     

    
