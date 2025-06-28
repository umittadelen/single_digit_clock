import sys
import subprocess

def install_package(package):
    subprocess.check_call([sys.executable, "-m", "pip", "install", package])

# Check tkinter availability
try:
    import tkinter as tk
    from tkinter import ttk, messagebox
except ImportError:
    input("""
❌ Tkinter is not installed or not available!

Please install it manually before running this script:

- On Debian/Ubuntu: sudo apt-get install python3-tk
- On Fedora: sudo dnf install python3-tkinter
- On Windows/macOS: usually pre-installed with Python.

Press ENTER key to exit...
""")
    sys.exit(1)

# Check pyserial availability
try:
    import serial
    import serial.tools.list_ports
except ImportError:
    print("Pyserial not found, installing it now...")
    try:
        install_package("pyserial")
        import serial
        import serial.tools.list_ports
        print("Pyserial installed successfully!")
    except Exception as e:
        print(f"Failed to install pyserial: {e}")
        sys.exit(1)
# ----------------------------------------------------------------------------------------
# 🕒 Arduino Time Setter GUI
# This script provides a responsive GUI to set the time of the RTC on an Arduino board.
# It includes features like COM port selection, real-time system time display, and serial output logging.
# ----------------------------------------------------------------------------------------
import tkinter as tk
from tkinter import ttk, messagebox
import serial
import serial.tools.list_ports
import datetime
import threading
import time

# Get available COM ports
def list_serial_ports():
    ports = serial.tools.list_ports.comports()
    return [f"{port.device} - {port.description}" for port in ports]

# Extract COM# from dropdown string
def extract_com_port(combo_text):
    return combo_text.split(" - ")[0].strip()

# Get current system time as HH:MM:SS
def get_current_time():
    return datetime.datetime.now().strftime("%H:%M:%S")

# Append message to output box
def log_message(msg):
    output_box.configure(state='normal')
    output_box.insert(tk.END, msg + '\n')
    output_box.see(tk.END)
    output_box.configure(state='disabled')

# Send system time to Arduino
def send_time():
    full_selection = port_var.get()
    if not full_selection:
        messagebox.showerror("No Port Selected", "Please select a COM port!")
        return

    selected_port = extract_com_port(full_selection)
    time_str = get_current_time()
    time_label_var.set(f"System Time: {time_str}")

    def serial_worker():
        try:
            ser = serial.Serial(selected_port, 115200, timeout=1)
            time.sleep(2)  # Allow Arduino to reset

            ser.write((time_str + '\n').encode())
            log_message(f"[→] Sent: {time_str}")
            time.sleep(0.1)

            response = ""
            start_time = time.time()
            while time.time() - start_time < 2:
                if ser.in_waiting:
                    response += ser.read(ser.in_waiting).decode(errors='ignore')
                time.sleep(0.1)

            ser.close()

            if response.strip():
                for line in response.strip().splitlines():
                    log_message(f"[←] {line.strip()}")
            else:
                log_message("[←] No response received 😢")

        except serial.SerialException as e:
            messagebox.showerror("Serial Error", f"Couldn't open {selected_port}:\n{e}")

    threading.Thread(target=serial_worker, daemon=True).start()

# Update the dropdown with fresh COM port list
def refresh_ports():
    port_dropdown['values'] = list_serial_ports()

# Update time label every second
def update_time_loop():
    time_label_var.set("System Time: " + get_current_time())
    root.after(1000, update_time_loop)

# 🧁 GUI setup
root = tk.Tk()
root.title("🕒 Arduino Time Setter (Responsive GUI!)")
root.minsize(500, 400)

# Grid weight for resizing
root.grid_columnconfigure(0, weight=1)
root.grid_rowconfigure(5, weight=1)

# COM Port Label + Dropdown
ttk.Label(root, text="Select COM Port:").grid(row=0, column=0, sticky="w", padx=10, pady=(10, 2))
port_var = tk.StringVar()
port_dropdown = ttk.Combobox(root, textvariable=port_var, values=list_serial_ports(), state="readonly")
port_dropdown.grid(row=1, column=0, sticky="ew", padx=10)
port_dropdown.focus_set()

# Time Label
time_label_var = tk.StringVar(value="System Time: " + get_current_time())
ttk.Label(root, textvariable=time_label_var, font=("Courier", 12)).grid(row=2, column=0, padx=10, pady=(10, 2), sticky="w")

# Buttons: Send & Refresh
button_frame = ttk.Frame(root)
button_frame.grid(row=3, column=0, sticky="ew", padx=10, pady=5)
button_frame.grid_columnconfigure((0, 1), weight=1)

ttk.Button(button_frame, text="⏩ Send Time", command=send_time).grid(row=0, column=0, sticky="ew", padx=2)
ttk.Button(button_frame, text="🔄 Refresh Ports", command=refresh_ports).grid(row=0, column=1, sticky="ew", padx=2)

# Output box label
ttk.Label(root, text="Serial Output:").grid(row=4, column=0, sticky="w", padx=10, pady=(10, 2))

# Output Text Box (expandable!)
output_box = tk.Text(root, height=10, bg="#f7f7f7", state='disabled', font=("Courier", 10), wrap="word")
output_box.grid(row=5, column=0, sticky="nsew", padx=10, pady=(0, 10))

# Scrollbar for output box
scrollbar = ttk.Scrollbar(root, command=output_box.yview)
output_box['yscrollcommand'] = scrollbar.set
scrollbar.grid(row=5, column=1, sticky='ns')

# Start system time loop
update_time_loop()

root.mainloop()
