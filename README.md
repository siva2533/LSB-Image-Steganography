# 📦 LSB Image Steganography (C Programming)

A simple and modular C-based steganography project to hide and extract secret data inside **24-bit BMP images** using the **Least Significant Bit (LSB)** technique. Clean CLI, magic string check, and friendly logs.

---

## 🚀 Features

### ✔ Encode Secret Data
- Hide a secret file inside a BMP image
- Uses **8 LSBs → 1 Byte**
- Custom **2-character magic string**
- Encodes, in order:
  - `Magic string`
  - `Secret file extension size`
  - `Secret file extension`
  - `Secret file size`
  - `Secret file data`

### ✔ Decode Secret Data
- Verifies magic string
- Extracts:
  - `Extension size`
  - `Extension`
  - `Secret size`
  - `Secret data`
- Auto-generates output filename with extension

### ✔ Clean & Safe
- Works only with **24-bit BMP**
- Image capacity check
- Copies BMP header as-is
- Colorful and detailed CLI messages

---

## 📂 Project Structure

├── main.c # CLI handling & operation selection

├── encode.c # Encoding logic (LSB)

├── encode.h # Encode function definitions

├── decode.c # Decoding logic (LSB)

├── decode.h # Decode function definitions

├── types.h # Custom enums & typedefs

├── common.h # Constants (magic length, suffix size)

└── README.md # This file

---

## 🔧 How It Works

### 🧠 Encoding (Hiding Data)
1. Read BMP header
2. Take magic string from user
3. Encode:
   - Magic string
   - Extension size
   - Extension
   - File size
   - File data
4. Copy remaining image buffer
5. Save output as stego image

### 🧠 Decoding (Extracting Data)
1. Ask for magic string
2. Verify magic from BMP
3. Decode:
   - Extension size → extension
   - Secret size → secret bytes
4. Dynamically create output file
5. Save extracted data

---

## ▶️ Command-Line Usage

### 🔐 Encode
./a.out -e <source.bmp> <secret.txt> <output.bmp>

**Example**
./a.out -e beautiful.bmp secret.txt hidden.bmp

### 🔓 Decode
./a.out -d <stego.bmp> <output_base_name>

**Example**
./a.out -d hidden.bmp decode_output

**If output name not given**
- Uses default base name `"default"`
- Adds extracted extension (like `.txt`, `.bin`)

(Handled inside `open_secret_file()` in `decode.c`)

---

## 🧪 Example Output (Encoding)

✔ beautiful.bmp is present
✔ secret.txt is present
🔍 Opening all files...
✔ Files opened successfully
✔ Image capacity checked successfully
✔ BMP header copied successfully
Enter magic string (max 2 chars): ok
✔ Magic string encoded successfully
✔ Secret file extension encoded
✔ Secret file size encoded
✔ Secret data encoded
🎉 Data encoded successfully!


---

## 🛠 Technologies Used

- **C Language**
- Bitwise operations (LSB encoding / decoding)
- File handling (`fopen`, `fread`, `fwrite`)
- BMP header parsing
- Modular programming

---

## ⚠️ Limitations

- Only supports **24-bit .BMP**
- Magic string length max = **2**
- Secret file must fit image capacity
- Uses LSB (no encryption)

---
