# 📸 Seam Carving – Content-Aware Image Resizing (C++ & OpenCV)

This repository implements **Seam Carving**, a content-aware image resizing algorithm, written entirely in **C++** without using C++ STL containers (as required). The project uses **OpenCV only** for image I/O and pixel extraction, while the seam carving algorithm itself is fully implemented manually.

The code resizes input images to user-specified dimensions by iteratively removing low-energy vertical or horizontal seams.

This repository contains the source code for the assignment as described in the problem statement.  
**Core file reference:** `2024201044_A1_Q4.cpp`

---

## 🚀 Features

Fully manual implementation of:

- Energy calculation using dual-gradient energy function  
- Vertical seam identification (Dynamic Programming)  
- Horizontal seam identification (Dynamic Programming)  
- Seam removal (pixel matrix reconstruction)

Uses OpenCV only for:

- Reading images  
- Extracting pixel RGB values  
- Writing the resized image  

Other key properties:

- No use of STL containers such as `vector`, as per assignment requirements  
- Supports interactive input for:
  - Image file name  
  - Desired output width  
  - Desired output height  
- Displays intermediary seam-highlight steps (optional visualization)  
- Saves final resized image as **`reduced_image.jpeg`**

---

## 📘 What is Seam Carving?

Seam carving is a **content-aware image resizing technique**. Instead of uniformly scaling an image, it removes the least important pixels using a dynamic programming approach.

A **seam** is defined as:

- **Vertical seam:** One pixel per row, connected from top → bottom  
- **Horizontal seam:** One pixel per column, connected from left → right  

**Algorithm Steps:**

1. Compute energy of every pixel  
2. Find the minimum-energy seam  
3. Remove that seam  
4. Repeat until the image reaches the desired size  

This produces resized images that preserve important structures better than naive resizing.

---

## 📂 Project Structure

```
├── sample_input/             # Images provided for testing (add manually)
├── 2024201044_A1_Q4.cpp      # Main source code (seam carving implementation)
├── README.md                 # Documentation
└── output/
    └── reduced_image.jpeg    # Automatically generated output
```

---

## 🔧 Installation & Setup

### 1. Install OpenCV (C++)

Follow assignment instructions, or use:

**Ubuntu:**
```bash
sudo apt-get install libopencv-dev
```

**Windows:**

- Download from https://opencv.org/releases/
- Configure include & library paths in your IDE (VSCode, CodeBlocks, Visual Studio, etc.)

---

## 🛠️ Compilation

Compile using g++ with OpenCV flags:

```bash
g++ 2024201044_A1_Q4.cpp -o seam_carving `pkg-config --cflags --libs opencv4`
```

If your system uses opencv instead of opencv4:

```bash
g++ 2024201044_A1_Q4.cpp -o seam_carving `pkg-config --cflags --libs opencv`
```

---

## ▶️ Running the Program

Run:

```bash
./seam_carving
```

You will be prompted for:

- **Input image name** (with extension)
  - Example: `sample1.jpeg`
- **Reduced width**
- **Reduced height**

During execution, seams may be shown in red for visualization.

The final output is saved as:

```
reduced_image.jpeg
```

---

## 📐 Algorithm Overview

### 1. Energy Calculation

Uses dual-gradient operator:

```
Energy = √[(dX)² + (dY)²]
```

Where:

- **dX** = difference between left and right pixels
- **dY** = difference between top and bottom pixels

Implemented in:

```cpp
double CalculateEnergy(Vec3b top, Vec3b down, Vec3b left, Vec3b right)
```

### 2. Vertical Seam Selection

Dynamic Programming matrix `dp` stores cumulative minimum energy from bottom → top.

Key function:

```cpp
int* VerticalSeamSelect(double **EnergyMatrix, int imgHeight, int imgWidth);
```

Output: list of column indices (one per row).

### 3. Horizontal Seam Selection

DP computed left → right.

Key function:

```cpp
int* HorizontalSeamSelect(double **EnergyMatrix, int imgHeight, int imgWidth);
```

Output: list of row indices (one per column).

### 4. Seam Removal

Image is reconstructed by skipping the seam pixel for each row/column.

Since STL is not allowed, manual dynamic array manipulation is used.

---

## 🖼️ Example (Visualization)

During processing, seams may appear in red `(0,0,255)`.

This helps visually verify that seam detection is working correctly.

---

## 📊 Performance Considerations

- DP seam computation is **O(H × W)** per seam
- Removing many seams increases compute time proportionally
- Memory manually handled using raw pointers
- Energy matrix is updated incrementally after each seam removal

---

## 📝 Requirements Fulfilled

✔ C++ only  
✔ Custom dynamic arrays (no STL vectors)  
✔ Custom seam carving algorithm  
✔ Uses OpenCV for pixel I/O only  
✔ Supports both vertical & horizontal resizing  
✔ Saves output image

---

## 🤝 Contributing

Potential enhancements:

- Better visualization
- Optimized energy functions
- Seam insertion (image expansion)
- Parallel processing

---

## 📄 License

This project is released under the MIT License.

---

## ⭐ Acknowledgments

This implementation is based on the algorithm from:

**Avidan & Shamir (2007)** — "Seam Carving for Content-Aware Image Resizing"