# 🚒 arduini: The Fire-Fighting Robot Car

We are building a little robot car that can **find a fire, drive up to it, and spray water on it**, all by itself!

The robot's brain is an **Arduino Uno**, a tiny computer about the size of a credit card. We tell it what to do by writing instructions in a programming language called **C++**.

> ⚠️ **Safety first:** only test the robot with a grown-up nearby, and use a small candle on a plate, away from anything that can burn.

---

## 🧩 What is the robot made of?

| Part | What it does | Like a... |
|------|--------------|-----------|
| **Arduino Uno** | Runs our code and makes all the decisions | 🧠 brain |
| **4 motors and wheels** | Drive the car forward, backward, and turn it around | 🦵 legs |
| **Flame sensor (5 eyes)** | Sees the invisible light that fire gives off | 👀 eyes |
| **Ultrasonic sensor** | Sends out a sound and listens for the echo to know how far away a wall is | 🦇 bat ears |
| **Water pump** | Squirts water at the fire | 🔫 water gun |
| **Colored light (RGB LED)** | Lights up so we can see what the robot is thinking | 💡 mood light |
| **Battery** | Gives everything power | 🔋 food |

The car comes from the **Zeus Car** kit. We started from its example programs and changed them to make a fire fighter.

---

## 👀 How does the robot find the fire?

Fire gives off a kind of light our eyes can't see, called **infrared light**. The flame sensor has **5 little eyes** in a row across the front of the robot:

```
          FRONT OF THE ROBOT
   [ 5 ]  [ 4 ]  [ 3 ]  [ 2 ]  [ 1 ]
   left          middle         right
```

Each eye answers a yes-or-no question: *"Do I see fire?"* The robot looks at which eyes said **yes** and turns until the fire is right in front of the **middle eye (3)**.

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

---

## 🗺️ The robot's plans (flowcharts)

A **flowchart** is a map of the robot's thinking. Start at the green box at the top and follow the arrows. Yellow diamonds are **questions**, and you follow the arrow with the answer (**YES** or **NO**).

| Flowchart | What it shows |
|-----------|---------------|
| [🔥 Flame detection plan](diagrams/Flame-Detection-Diagram.drawio.svg) | Our very first idea, drawn before we wrote any code |
| [🧱 Nov 2: Wall Dodger](diagrams/nov2-wall-dodger.drawio.svg) | The robot drives around and steers away from walls |
| [🧭 Nov 8: Fire Finder](diagrams/nov8-fire-finder.drawio.svg) | The robot turns to face the fire, but doesn't drive yet |
| [🚒 Nov 14: Fire Fighter](diagrams/nov14-fire-fighter.drawio.svg) | The whole robot: find the fire, get close, spray, and avoid walls |
| [💦 Nov 14: Spray](diagrams/nov14-spray.drawio.svg) | Zoomed in on how the robot sprays water |

### 🧱 Nov 2: Wall Dodger

Before the robot learned to chase fire, it learned to drive without crashing. It drives forward, and when its bat ears hear something closer than 10 cm, it stops and turns right for a surprise amount of time. Then it checks again until the way is clear. It also looks for fire, but only reports what it sees and doesn't do anything about it yet.
➡️ [See the flowchart](diagrams/nov2-wall-dodger.drawio.svg) · [See the code](src/examples/driveArounds/move_speed__nov2.ino)

### 🧭 Nov 8: Fire Finder

The robot sits still and keeps looking for fire. When an eye sees fire, the robot turns toward it, like a sunflower turning to face the sun. 🌻
➡️ [See the flowchart](diagrams/nov8-fire-finder.drawio.svg) · [See the code](src/examples/driveArounds/driveAround_10242026_nov8.ino)

### 🚒 Nov 14: Fire Fighter

This is the full fire fighter! Over and over, the robot:

1. **Looks** for fire with its 5 eyes.
2. **Turns** to face the fire.
3. **Checks how bright the fire looks.** The closer the robot is, the brighter it looks.
   - Too bright? It's **too close**, so it backs up a little.
   - Too dim? It's **too far**, so it moves forward a little.
   - Just right? 💦 **SPRAY!**
4. **Listens for walls** with its bat ears. If something is closer than 11 cm (about the length of your finger), it stops and turns right for a surprise amount of time, so it doesn't get stuck in the same spot.
5. If there's no wall, it **drives forward** to explore.

➡️ [See the flowchart](diagrams/nov14-fire-fighter.drawio.svg) · [See the code](src/examples/driveArounds/driveAround_10242026_Nov14.ino)

### 💦 How the robot sprays

The robot doesn't just squirt once. It sprays in **3 directions** so it covers the whole flame:

1. Squirt straight ahead.
2. Turn a little to the right and squirt.
3. Turn a bit to the left and squirt.
4. Turn back to the middle.

➡️ [See the flowchart](diagrams/nov14-spray.drawio.svg)

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
│   └── driveArounds/
│       ├── driveAround_10242026.ino       Our first drive-around test
│       ├── move_speed__nov2.ino           Nov 2: Wall Dodger
│       ├── driveAround_10242026_nov8.ino  Nov 8: Fire Finder
│       └── driveAround_10242026_Nov14.ino Nov 14: Fire Fighter
└── diagrams/                  ← 🗺️ FLOWCHARTS (maps of the robot's thinking)
    ├── Flame-Detection-Diagram.drawio(.svg)
    ├── nov2-wall-dodger.drawio(.svg)
    ├── nov8-fire-finder.drawio(.svg)
    ├── nov14-fire-fighter.drawio(.svg)
    └── nov14-spray.drawio(.svg)
```

Each flowchart comes in two files:
- **`.drawio.svg`** is the picture. Click it on GitHub to see it.
- **`.drawio`** is the same drawing for editing. Open it at [app.diagrams.net](https://app.diagrams.net) to change it. (You can also edit the `.drawio.svg` there.)

---

## 🔌 How to put the code on the robot

1. Install the free [**Arduino IDE**](https://www.arduino.cc/en/software) on your computer.
2. In the Arduino IDE, open **Library Manager**, search for **SoftPWM**, and click **Install**. The robot needs it to control its motors and lights.
3. Open the `.ino` file you want. The Arduino IDE likes every `.ino` file to live in a folder with the **same name**. If it asks to make one, click **OK**.
4. Plug the Arduino Uno into your computer with a USB cable.
5. Choose **Tools → Board → Arduino Uno** and pick the right **Port**.
6. Click the **➡️ Upload** button and wait for "Done uploading".
7. Unplug the USB cable, turn on the robot's battery, and watch it go! 🎉

---

## 🧠 Words to know

- **Code**: instructions for a computer, written so it can understand them.
- **Sensor**: a part that lets the robot feel or see something (fire, walls).
- **Loop**: doing the same steps again and again, forever. Our robot checks for fire many times every second!
- **Flowchart**: a drawing with boxes and arrows that shows the steps of a plan.
- **Infrared**: a kind of light that's invisible to people but that the flame sensor can see.
