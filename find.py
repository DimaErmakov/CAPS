import bluetooth


def connect_to_device(address, port):
    sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    sock.connect((address, port))
    print(f"Connected to {address} on port {port}")
    return sock


def find_bluetooth_devices():
    devices = bluetooth.discover_devices(lookup_names=True)
    for addr, name in devices:
        print(f"Device: {name}, Address: {addr}")


if __name__ == "__main__":
    find_bluetooth_devices()
    deviceAddress = "7C:ED:C6:C9:2E:18"
