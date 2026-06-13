# **Chronos \- A Mechanical Pomodoro Timer and Clock**

![Final clock render](README%20Images/Final%20Clock%20Render.png)

[Onshape CAD Link](https://cad.onshape.com/documents/4e46eab36ada7087591254e1/w/535417db20a99f0700b6b619/e/2bade0778d34ed4b7359ccd4?renderMode=0&uiState=6a28f4a51491e7cf1e2738ea)

![Zine](README%20Images/Chronos-Zine)

# **Overview**

Chronos is a 7-Segment mechanical display that is supposed to function as a pomodoro timer,  clock, or as a regular timer. It’s equipped with speakers that play an alarm noise when time is up and also random sound effects throughout the timer. The speakers let the user know how many pomodoro cycles they have completed after each cycle. They also help Chronos function as an alarm clock when it is put into clock mode. On the top there are two buttons and a rotary encoder which serve as inputs to control Chronos. Chronos is powered by a ESP32-C3 on a custom PCB that drives all the electronics. This device is named Chronos because in Greek it means the quantitative measurement of time that ticks away on a clock.

# **Why does this exist?**

Chronos was mainly built to function as a pomodoro timer, but since it has four numerical displays I decided to make it usable as a clock and regular timer as well. In case you don’t know what a Pomodoro timer is, it’s a simple timer that is used with the Pomodoro Technique. The Pomodoro Technique is a study strategy where you work in short focused intervals (usually 25 minutes) followed by brief breaks (about 5 minutes). You repeat this cycle four times and then you take a longer 15-30 minute break. It’s meant to help you stay focused, reduce procrastination, and make long tasks feel more manageable by splitting them into these repeating cycles. As a student myself who struggled with procrastination and locking in during study sessions I found the pomodoro technique to be extremely useful. I mainly used online timers I found but I found them to be kind of boring so I came up with this device. It’s extremely satisfying because it makes a click every time a digit changes and it’s also cool to see as I study. It’s kind of like an ASMR sound effect while I work or study.

# **Mechanics**

The clock works through four display modules each with seven segments. Each of these 7 segments are connected to a rack and pinion which drives each segment and controls when each segment is displayed and not displayed.  

![Rack & Pinion Breakdown](README%20Images/rack-and-pinion-breakdown.png)  

Each of these racks are driven by a cam and follower mechanism that pushes each rack individually. There is a stack of 14 cams and each follower has 2 cams. One to retract it and one to extend it. Depending on the angle of the shaft different cams are extended and different cams are retracted which control the number displayed. Numbers are displayed at a 36 degree increment of the shaft. For example at 0 degrees the number 0 is displayed and at 72 degrees the number 2 is displayed.

![](README%20Images/pomoclock%20cams.png)
![](README%20Images/CAM-breakdown)  

The shaft is driven by a servo that is geared to it. 

![](README%20Images/pomoclock%20gears.png)

# **Electronics**

This device is powered by a custom PCB I made in KiCAD that utilizes a Seed Studio XIAO ESP32-C3. On my board I have 4 servo pins which are driven by PWM signals from my microcontroller and the 5V power. There are also two MAX98357A Audio Drive ICs used to control the speakers on my device. I also have a 8-bit IO Expander IC because there weren’t enough GPIO pins on my microcontroller. The 8-Bit IO Expander communicates with my microcontroller through I2C and controls my rotary encoder switch and my two linear switches. The PCB is powered by a 8A 5V AC-DC unit that I plug into the wall and connect into my barrel jack connector. From there the power goes into a reverse polarity mosfet circuit that blocks current if the power supply is connected backwards. This P channel mosfet is then connected to my bulk 2200uF bulk capacitor that acts like a power reservoir to prevent brownouts in case the servos suddenly stall.  

![](README%20Images/pcb%20layout.png)

![](README%20Images/pcb%20w%20components.png)

# **Control**

There are 3 methods of input into the clock. A rotary encoder switch, and two cherry mx style linear mechanical keyboard switches. When you rotate the white knob in the middle you can change the duration of the timer when you are in the pomodoro or timer mode. When you click it you can pause or resume the clock. The right button lets you switch between clock mode, pomodoro mode, and timer mode. The left button lets you skip between pomodoro cycles.  

![](README%20Images/rotary%20encoder.png)
EC11EA5 Rotary Encoder Switch  

![](README%20Images/linear%20switch.png)
Cherry MX Style Linear Mechanical Keyboard Switch 
