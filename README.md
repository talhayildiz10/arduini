# 🚒 arduini: The Fire-Fighting Robot Car

We are building a little robot car that can **find a fire, drive up to it, and spray water on it**, all by itself!

The robot's brain is an **Arduino Uno**, a tiny computer about the size of a credit card. We tell it what to do by writing instructions in a programming language called **C++**.

> ⚠️ **Safety first:** only test the robot with a grown-up nearby. Use one small candle on a plate, away from paper, curtains, and anything else that can burn.

**🏁 Want to jump right in?** The newest robot is the [**Nov 14 Fire Fighter**](#-nov-14-fire-fighter). Look at its [flowchart](diagrams/nov14-fire-fighter.drawio.svg), then [put the code on the robot](#-put-the-code-on-the-robot).

## 📖 Contents

1. [What is the robot made of?](#-what-is-the-robot-made-of)
2. [How does the robot find the fire?](#-how-does-the-robot-find-the-fire)
3. [How to read a flowchart](#%EF%B8%8F-how-to-read-a-flowchart)
4. [Our robot's story](#-our-robots-story)
5. [Put the code on the robot](#-put-the-code-on-the-robot)
6. [Try it yourself!](#-try-it-yourself)
7. [Bug hunt](#-bug-hunt)
8. [Where everything is](#-where-everything-is)
9. [Words to know](#-words-to-know)
10. [For grown-ups: wiring](#-for-grown-ups-wiring)

---

## 🧩 What is the robot made of?

| Part | What it does | Like a... |
|------|--------------|-----------|
| **Arduino Uno** | Runs our code and makes all the decisions | 🧠 brain |
| **4 motors and wheels** | Drive the car forward, backward, and spin it around | 🦵 legs |
| **Flame sensor (5 eyes)** | Sees the invisible light that fire gives off | 👀 eyes |
| **Ultrasonic sensor** | Sends out a sound and listens for the echo to know how far away a wall is | 🦇 bat ears |
| **Water pump** | Squirts water at the fire | 🔫 water gun |
| **Colored light (RGB LED)** | Lights up so we can see what the robot is thinking | 💡 mood light |
| **Battery** | Gives everything power | 🔋 food |

The car comes from the **Zeus Car** kit. We started from its example programs and changed them, step by step, into a fire fighter.

---

## 👀 How does the robot find the fire?

Fire gives off a kind of light our eyes can't see, called **infrared light**. The flame sensor has **5 little eyes** in a row across the front of the robot:

```
          FRONT OF THE ROBOT
   [ 5 ]  [ 4 ]  [ 3 ]  [ 2 ]  [ 1 ]
   left          middle         right
```

Each eye answers a yes-or-no question: *"Do I see fire?"* The robot looks at which eyes said **yes** and turns until the fire is right in front of the **middle eye (3)**. The further the fire is to one side, the more the robot turns.

| Eyes that see fire | What the robot does |
|--------------------|---------------------|
| Only 1 | Turn **right** a lot |
| 1 and 2 | Turn **right** quite a bit |
| Only 2, or 1, 2 and 3 | Turn **right** a little |
| 2 and 3, or 1, 2, 3 and 4 | Turn **right** a tiny bit |
| Only 3, or 2, 3 and 4, or all 5 | 🎯 Stay still: the fire is straight ahead! |
| 3 and 4, or 2, 3, 4 and 5 | Turn **left** a tiny bit |
| Only 4, or 3, 4 and 5 | Turn **left** a little |
| 4 and 5 | Turn **left** quite a bit |
| Only 5 | Turn **left** a lot |

**How does it know how close it is?** The middle eye can also tell **how bright** the fire is, as a number from 0 to 1023. The closer the robot gets, the bigger the number.

---

## 🗺️ How to read a flowchart

A **flowchart** is a map of the robot's thinking. Each one is a picture, so click its link to see it.

- 🟩 **Green box:** start here.
- 🟦 **Blue box:** the robot *does* something.
- 🟨 **Yellow diamond:** the robot *asks a question*. Follow the arrow with the answer (**YES** or **NO**).
- 🟥 **Red box:** something important happened (fire found, or a wall ahead!).
- 💧 **Water-blue box:** spraying time.

When an arrow goes back up, the robot **repeats** those steps. That's called a **loop**.

---

## 📅 Our robot's story

We built the robot a little at a time. Each version learned a new trick.

### 🔥 The first plan

Before writing any code, we drew our idea: look for heat, and if there's no fire, wander around without hitting walls.
➡️ [Flowchart](diagrams/Flame-Detection-Diagram.drawio.svg)

### 🚗 Oct 24: First drive

The first test after we built the car. We tried the Zeus Car examples to make sure the wheels and sensors worked:
[driving](src/examples/1_basic_move.ino), [spinning](src/examples/3_rotate_and_move.ino), [measuring distance](src/examples/10_ultrasonic.ino), and [our first drive-around](src/examples/driveArounds/driveAround_10242026.ino).

### 🧱 Nov 2: Wall Dodger

The robot learned to drive without crashing. It drives forward, and when its bat ears hear something closer than **10 cm**, it stops and turns right for a surprise amount of time. Then it checks again until the way is clear. It also looks for fire, but only reports what it sees.
➡️ [Flowchart](diagrams/nov2-wall-dodger.drawio.svg) · [Code](src/examples/driveArounds/move_speed__nov2.ino)

### 🧭 Nov 8: Fire Finder

The robot learned to **face the fire**. It sits still and keeps looking. When an eye sees fire, the robot turns toward it, like a sunflower turning to face the sun. 🌻 It doesn't drive in this version.
➡️ [Flowchart](diagrams/nov8-fire-finder.drawio.svg) · [Code](src/examples/driveArounds/driveAround_10242026_nov8.ino)

### 🚒 Nov 14: Fire Fighter

Everything together! Over and over, the robot:

1. **Looks** for fire with its 5 eyes.
2. **Turns** to face the fire.
3. **Checks how bright the fire looks:**
   - Too bright (more than 700)? It's **too close**, so it backs up a little.
   - Too dim (less than 400)? It's **too far**, so it moves forward a little.
   - Just right? 💦 **SPRAY!**
4. **Listens for walls.** If something is closer than **11 cm** (about the length of your finger), it stops and turns right for a surprise amount of time.
5. If there's no wall, it **drives forward** to explore.

➡️ [Flowchart](diagrams/nov14-fire-fighter.drawio.svg) · [Code](src/examples/driveArounds/driveAround_10242026_Nov14.ino)

**💦 How it sprays:** the robot doesn't just squirt once. It sprays in **3 directions** to cover the whole flame: straight ahead, a little to the right, then a little to the left. Then it turns back to the middle.
➡️ [Spray flowchart](diagrams/nov14-spray.drawio.svg)

---

## 🔌 Put the code on the robot

1. Install the free [**Arduino IDE**](https://www.arduino.cc/en/software) on your computer.
2. Click **Tools → Manage Libraries**, search for **SoftPWM**, and click **Install**. The robot needs it to control its motors and lights.
3. Click **File → New Sketch** and delete everything in the new window.
4. Open the code you want on GitHub (for example, [Nov 14 Fire Fighter](src/examples/driveArounds/driveAround_10242026_Nov14.ino)), copy all of it, and paste it into the Arduino IDE.
5. Plug the Arduino Uno into your computer with a USB cable.
6. Choose **Tools → Board → Arduino Uno**, and under **Tools → Port** pick the one that says Arduino.
7. Click the **➡️ Upload** button and wait for "Done uploading".
8. Unplug the USB cable, turn on the robot's battery, and watch it go! 🎉

> 💡 Why not just open the file? The Arduino IDE wants every program in its own folder with the same name. Our programs share one folder, so the IDE would try to move files around. Copying and pasting into a new sketch keeps our project tidy.

---

## 🧪 Try it yourself!

Small changes make a big difference. In the [Nov 14 code](src/examples/driveArounds/driveAround_10242026_Nov14.ino), find these lines, change the number, upload again, and watch what happens.

| Find this | It means | Try changing it to |
|-----------|----------|--------------------|
| `#define ULTRASONIC_AVOIDANCE_THRESHOLD 11` | How close a wall can get (in cm) before the robot turns away | `20`: the robot turns away from walls much earlier |
| `int8_t power_driveForward = 28;` | How fast the robot drives | `40`: faster! |
| `uint8_t sprayTime = 200;` | How long each squirt lasts (in milliseconds, so 200 = one fifth of a second) | `400`: twice as much water |
| `#define MAX_TURN_ANGLE 1300` | The longest "surprise turn" after seeing a wall | `600`: smaller, shorter turns |

📝 Write down what you changed, so you can change it back if you don't like it!

---

## 🐞 Bug hunt

Real programmers find bugs all the time. Here are two we found. Can you fix them?

1. **The lights show the wrong colors.** Computers start counting at **0**, but our code starts the color list at **1**. So when we ask for red, we get orange, and pink comes out white! The fix is in the `color_show` function: change `key_color_array[index]` to `key_color_array[index - 1]`.
2. **The Nov 2 code won't upload.** [`move_speed__nov2.ino`](src/examples/driveArounds/move_speed__nov2.ino) has the whole program pasted **twice**, and the computer gets confused when two things have the same name. The fix is to delete the second copy: on line 370, keep the `}` at the start and delete everything after it, to the end of the file.

---

## 📁 Where everything is

We keep **code** and **diagrams** in separate folders, so they're easy to find.

```
arduini/
├── README.md                  ← you are here!
├── src/examples/              ← 💻 CODE (the robot's instructions)
│   ├── 1_basic_move.ino           Zeus Car example: drive in all directions
│   ├── 3_rotate_and_move.ino      Zeus Car example: spin and drive
│   ├── 10_ultrasonic.ino          Zeus Car example: measure distance
│   └── driveArounds/          ← our own programs
│       ├── driveAround_10242026.ino       Oct 24: First drive
│       ├── move_speed__nov2.ino           Nov 2:  Wall Dodger
│       ├── driveAround_10242026_nov8.ino  Nov 8:  Fire Finder
│       └── driveAround_10242026_Nov14.ino Nov 14: Fire Fighter
└── diagrams/                  ← 🗺️ FLOWCHARTS (maps of the robot's thinking)
    ├── Flame-Detection-Diagram.drawio(.svg)   The first plan
    ├── nov2-wall-dodger.drawio(.svg)
    ├── nov8-fire-finder.drawio(.svg)
    ├── nov14-fire-fighter.drawio(.svg)
    └── nov14-spray.drawio(.svg)
```

Each flowchart comes in two files:
- **`.drawio.svg`** is the picture. Click it on GitHub to see it.
- **`.drawio`** is the same drawing for editing. Open it at [app.diagrams.net](https://app.diagrams.net) to change it. You can also open and edit the `.drawio.svg` there.

---

## 🧠 Words to know

- **Code**: instructions for a computer, written so it can understand them.
- **C++**: the programming language we write our robot's code in.
- **Sketch**: what Arduino calls a program. Our sketches end in `.ino`.
- **Sensor**: a part that lets the robot feel or see something (fire, walls).
- **Loop**: doing the same steps again and again. Our robot checks for fire many times every second!
- **Flowchart**: a drawing with boxes and arrows that shows the steps of a plan.
- **Infrared**: a kind of light that's invisible to people but that the flame sensor can see.
- **Millisecond (ms)**: one thousandth of a second. 1000 ms = 1 second.
- **Bug**: a mistake in the code that makes the robot do something we didn't want.

---

## 🔧 For grown-ups: wiring

These are the Arduino Uno pins our sketches in `driveArounds/` expect. Every sketch that uses a part uses the same pins for it. Only Nov 14 uses the pump, and the Oct 24 sketch has no flame sensor.

| Part | Pin(s) |
|------|--------|
| Flame sensor digital outputs, eyes 1 to 5 | D7, D8, D9, D2, D0 |
| Flame sensor analog output (brightness) | A4 |
| Ultrasonic sensor (trigger and echo share a pin) | D10 |
| Water pump enable | A5 |
| RGB LED (red, green, blue) | D12, D13, D11 |
| Motors (2 pins each, 4 motors) | D3, D4, D5, D6, A3, A2, A1, A0 |

> Note: D0 is also the Arduino's USB serial receive pin, and the sketches use the Serial Monitor. If eye 5 behaves oddly while the USB cable is plugged in, this pin sharing is a likely cause.

**Library needed:** [SoftPWM](https://github.com/bhagman/SoftPWM) (install it from the Arduino IDE's Library Manager).
