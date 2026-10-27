
import pygame
import socket

pygame.init()

# ESP32 Bluetooth MAC address
esp32_address = "5C:01:3B:97:69:02"

# Bluetooth RFCOMM connection
esp32 = socket.socket(
    socket.AF_BLUETOOTH,
    socket.SOCK_STREAM,
    socket.BTPROTO_RFCOMM
)

esp32.connect((esp32_address, 1))

# Pygame window
screen = pygame.display.set_mode((400, 200))
pygame.display.set_caption("ESP32 Motor Control")

clock = pygame.time.Clock()
running = True

try:
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False

        keys = pygame.key.get_pressed()

        left_y = 0.0
        right_y = 0.0

        # W = Forward
        if keys[pygame.K_w] and not keys[pygame.K_s]:
            left_y = -1.0
            right_y = 1.0


        # A right
        if keys[pygame.K_a] and not keys[pygame.K_s]:
                    left_y = -1.0

        # D Left
        if keys[pygame.K_d] and not keys[pygame.K_s]:
                    right_y = 1.0



        # S = Reverse
        elif keys[pygame.K_s] and not keys[pygame.K_w]:
            left_y = 1.0
            right_y = -1.0

        # Send values to ESP32
        data = f"{left_y:.2f},{right_y:.2f}\n"
        esp32.sendall(data.encode())

        print(f"Left: {left_y:.2f} | Right: {right_y:.2f}")

        clock.tick(20)

finally:
    # Stop motors
    try:
        esp32.sendall(b"0.00,0.00\n")
    except OSError:
        pass

    esp32.close()
    pygame.quit()
