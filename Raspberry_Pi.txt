import socket
import time
import serial
import cv2
import numpy as np
import threading

lock = threading.Lock()
#ser = serial.Serial('/dev/serial0', 115200)  # Uncomment this line if using serial
ESP32_IP = '192.168.4.1'
PORT = 80

print("ESP32 서버에 연결됨1")

print("Start")
#ser.write("Start\n".encode())

key = ''
alpha = 0.5
cap = cv2.VideoCapture(0)

color = 'O'

def send_data():
    global color
    print("ready to send")
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((ESP32_IP, PORT))
        sock = s    
        sock.sendall((color + '\n').encode())
        print("메시지 보냄:", color)
        time.sleep(0.3)
    except Exception as e:
         print(e)
    finally:
	    try:
	        sock.close()
	    except:
	        pass
	    lock.release()

def main():
    global color

    while True:
        ret, frame = cap.read()
        if not ret:
            break
        color = 'e'


        img_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        lower_red1 = np.array([0, 80, 50])
        upper_red1 = np.array([8, 255, 255])
        lower_red2 = np.array([172, 80, 50])
        upper_red2 = np.array([180, 255, 255])

        lower_blue = np.array([100, 100, 50])
        upper_blue = np.array([130, 255, 255])

        lower_white = np.array([0, 0, 200])
        upper_white = np.array([180, 40, 255])

        lower_black = np.array([0, 0, 0])
        upper_black = np.array([180, 255, 50])

        red_mask1 = cv2.inRange(hsv_frame, lower_red1, upper_red1)
        red_mask2 = cv2.inRange(hsv_frame, lower_red2, upper_red2)
        red_mask = cv2.bitwise_or(red_mask1, red_mask2)

        red_objects = cv2.bitwise_and(frame, frame, mask=red_mask)

        blue_mask = cv2.inRange(hsv_frame, lower_blue, upper_blue)
        blue_objects = cv2.bitwise_and(frame, frame, mask=blue_mask)

        black_mask = cv2.inRange(hsv_frame, lower_black, upper_black)
        black_objects = cv2.bitwise_and(frame, frame, mask=black_mask)
        white_mask = cv2.inRange(hsv_frame, lower_white, upper_white)
        white_objects = cv2.bitwise_and(frame, frame, mask=white_mask)

        total_mask = cv2.bitwise_or(red_mask, blue_mask)
        total_mask = cv2.bitwise_or(total_mask, black_mask)
        total_mask = cv2.bitwise_or(total_mask, white_mask)

        other_mask = cv2.bitwise_not(total_mask)
        other_objects = cv2.bitwise_and(frame, frame, mask=other_mask)

        red_contours, _ = cv2.findContours(red_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        blue_contours, _ = cv2.findContours(blue_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        white_contours, _ = cv2.findContours(white_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        black_contours, _ = cv2.findContours(black_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        other_contours, _ = cv2.findContours(other_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        if len(other_contours) > 0:
            for contour in other_contours:
                area = cv2.contourArea(contour)
                (x, y), radius = cv2.minEnclosingCircle(contour)

                if radius > 20 and (380 < y < 450) and (230 < x < 530) and area > 2000:
                    cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 0), 2)
                    print('cc')
                    color = 'C'
                    if lock.acquire(blocking=False):
                        threading.Thread(target=send_data, daemon=True).start()

        if len(red_contours) > 0:
            for contour in red_contours:
                area = cv2.contourArea(contour)
                (x, y), radius = cv2.minEnclosingCircle(contour)

                if radius > 20 and (0 < y < 30) and (270 < x < 460) and area > 2000:
                    cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 0), 2)
                    color = 'R'
                    print('rr')
                    if lock.acquire(blocking=False):
                        threading.Thread(target=send_data, daemon=True).start()


        if len(blue_contours) > 0:
            for contour in blue_contours:
                area = cv2.contourArea(contour)
                (x, y), radius = cv2.minEnclosingCircle(contour)

                if radius > 20 and (0 < y < 30) and (270 < x < 460)and area > 2000:
                    cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 0), 2)
                    color = 'B'
                    print('bb')
                    if lock.acquire(blocking=False):
                        threading.Thread(target=send_data, daemon=True).start()

        
        print(color)


        cv2.imshow('mask', red_mask)
        cv2.imshow('blue mask', blue_mask)

        overlay1 = frame.copy()
        cv2.rectangle(frame, (270, 0), (460, 30), (255, 255, 255), -1)
        cv2.rectangle(frame, (230, 380), (530, 450), (255, 255, 255), -1)
        blended = cv2.addWeighted(overlay1, alpha, frame, 1 - alpha, 0)

        cv2.imshow("Camera Preview", blended)
        cv2.imshow("mask2",other_mask)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()

    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()



cap.release()
cv2.destroyAllWindows()
