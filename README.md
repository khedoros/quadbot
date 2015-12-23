QuadBot
=======

Purpose
-------
I'm building a quadrupedal robot (think a Starcraft Dragoon, not a robot dog), with the intention of eventually using it as a platform to play with image recognition and control by an AI. The first version of the code is built to be controlled by a bluetooth gamepad.

Architecture
------------
### Hardware
I'm using a Raspberry Pi model A+ with an [Adafruit PWM hat](https://www.adafruit.com/products/2327) (based on an NXP PCA9685 IIC bus 16ch LED controller) to offload servo timing to actual hardware. The Pi provides network, bluetooth, and USB capabilities for future extensibility, and the servo hat avoids glitchiness stemming from using a non-realtime OS (i.e. Linux) in a partially-realtime application. The robot itself uses 12 9g micro servos among 4 legs, offering 3 degrees of freedom per leg (one to swing the hip, one to raise the hip, one to raise the knee). The body and leg components are cut from birch plywood. Obviously, some of these details impact the software itself, and some are just implementation-specific design choices. I'd expect the software to be useful for building a Pi-based quadruped, regardless of the actual scale or materials used for the robot itself.

### Software
Control interface -> body/gait+timing+movement control -> individual legs/servo position interpolation/IK -> individual servos

The initial "Control interface" will use the Linux joystick API because I end up with a /dev/input/js0 when I connect an Ouya control over bluetooth. The output will be higher-level commands to the "body" component (like "set height to 5cm", "move forward", "set tilt 20 degrees left", or "turn CW"). Those will be translated into speed and foot positions and sent to the legs. The legs determine the actual angles that the servos need to be at (using a non-general, trigonometry-based IK calculation). The servos will have individual trim/min/max information, and will translate an angle into a PWM timing value and output the positioning values to the PWM controller.
