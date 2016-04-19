# Greatest Monkey Show on Earth

Custom-designed remote-controlled buzzer with LEDs. IR reader responds to the OK/MUTE button of the remote.

[![Custom buzzer](http://i.imgur.com/pSZWc4n.jpg?2)](https://www.youtube.com/watch?v=mZtTcWTQRRo "Click to see the video")

## Instructions
1. Turn the switch to "on"
2. Hit the "ok/mute" button on the remote controll
3. The LEDs will light up, and the buzzer will sound!
4. BE CAREFUL. This circuit has a Lithium-Ion battery. If this battery is punctured or damaged IT COULD EXPLODE.
5. To charge, connect a micro-USB cable to the micro-USB port. When the battery is fully charged, a light on the back will glow green.
6. With a fully charged battery, this should last at least 3 hours (but probably more).

![Circuit view](http://i.imgur.com/TDTeM19.jpg?2)

![All shipped off!](http://i.imgur.com/HroYwX8.jpg?2)

Notes:
IR signal captured using raw_ir_receive.
Tone produced (ab)using this [ToneAC library](https://bitbucket.org/teckel12/arduino-toneac/wiki/Home) because of [this Multiple Definition of "__vector_7" Error](https://bitbucket.org/teckel12/arduino-new-ping/wiki/Multiple%20Definition%20of%20%22__vector_7%22%20Error) when trying to use the built-in tone library.
[Possible ways to make it louder.](http://electronics.stackexchange.com/questions/161820/loudest-piezo-with-low-voltage-and-current)
[Buzzer + LEDs](http://www.alibaba.com/product-detail/Matek-WS2812B-LED-5V-Active-Buzzer_60303713205.html?spm=a2700.7724857.0.0.U3pdfQ)
Replaced the original buzzer above with this [Piezo Buzzer from Sparkfun.](https://www.sparkfun.com/products/7950) (where users say: a) It’s loudest at 2048Hz, and b) drive it with a transistor and include the diode (see schematic in datasheet))
