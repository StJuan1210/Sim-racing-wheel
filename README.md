# Sim-racing-wheel
F1 style sim racing wheel with pedals and paddleshifters
![IMG_20210607_171528](https://user-images.githubusercontent.com/86282256/129944784-871a32f5-b26b-4733-afdb-b64c0f9823df.jpg)
![Screenshot_20210610-121506](https://user-images.githubusercontent.com/86282256/129944869-4a946111-ef68-4ff1-aa33-6e4a41c37a23.jpg)
The first prototype


https://youtu.be/-DbteEQGfMg
-working video

The project is based on the arduino xinput library and requires a board like the leonardo.
The steering wheel uses a gyroscope for steering angle.
The pedals are from a old sewing machines(these pedals are basically giant potentiometers, so just cut open the wires and link them to arduino).
limit switches are there for paddle shifters.
For the display an old android phone is used which is displaying telemtry with the help of the simhub software.
the led strip is connected to a seperate arduino(this also uses simhub).
The circuit diagram is in the project files.
