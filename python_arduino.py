import serial
import tkinter as tk

ser = serial.Serial('COM3', 57600)

def enviar_mensaje(event=None):
    mensaje = entrada.get()
    if mensaje.lower() == 'bye':
        ser.close()
        root.destroy()
    else:
        ser.write(mensaje.encode() + b"\n")
        entrada.delete(0, 'end')

def borrar_texto():
    entrada.delete(0, 'end')

def finalizar_programa():
    ser.close()
    root.destroy()

root = tk.Tk()

root.minsize(width=250, height=100)
root.maxsize(width=1000, height=300)

entrada = tk.Entry(root)
entrada.pack(pady=10)
entrada.bind('<Return>', enviar_mensaje)

frame_botones = tk.Frame(root)
frame_botones.pack()

boton_enviar = tk.Button(frame_botones, text="Enviar", command=enviar_mensaje)
boton_enviar.pack(side=tk.LEFT, padx=5)

boton_borrar = tk.Button(frame_botones, text="Borrar", command=borrar_texto)
boton_borrar.pack(side=tk.RIGHT, padx=5)

boton_finalizar = tk.Button(root, text="Finalizar", command=finalizar_programa)
boton_finalizar.pack(pady=10)

# Center the window on the screen
root.update_idletasks()
width = root.winfo_width()
height = root.winfo_height()
x = (root.winfo_screenwidth() - width) // 2
y = (root.winfo_screenheight() - height) // 2
root.geometry(f'{width}x{height}+{x}+{y}')

root.mainloop()
