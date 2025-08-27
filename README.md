![KakaoTalk_20250609_092816668_07](https://github.com/user-attachments/assets/694eb413-eb6d-487f-8d2f-f794eef6d55d)



<img width="200" height="200" alt="image" src="https://github.com/user-attachments/assets/94a330c0-445c-42fc-982e-6b56f97ef9af" />


<img width="200" height="200" alt="image" src="https://github.com/user-attachments/assets/8a38bcb4-00a8-4d8d-ab16-319abfa60f43" />


flow chart설명

1.라즈베리파이 및 ESP32으로의 전원 공급.

2. 소켓 통신 연결
      -esp32 <-> 라즈베리파이 연결
      -연결 시, 라즈베리파이에 연결된 카메라 모듈 open
      -시리얼 통신을 통해 esp32측으로 "START" 신호 전달

3. 컨베이어 벨트 구동
      -라즈베리파이로 부터 받은 START 신호를 확인 후 스태핑 모터를 구동시켜 컨베이어 벨트를 따라
        물체 운송

4. 카메라 영상 색깔 식별 및 값 반환
      -컨베이어 벨트로 Object를 카메라 모듈 인식 영역까지 운송
      -카메라는 인식 영역 내에서 해당 Object의 색깔을 감지하고 
        사전에 주어진 조건에 따라 구분하여, A(Red)/B(Blue)/C(그외) 중 하나의 값을 반환.
      -소켓 통신을 통해 ESP32로 해당 값을 전송.

5. 운송 경로 변경
       -라즈베리파이로부터 값을 받은 ESP32는 값에 따른 조건에 따라 
           서보모터를 동작시켜, A/B/C Box로 운송할 수 있도록 운동 경로 변경

6.구동 정지
        -라즈베리파이와 ESP32 간 소켓 통신이 끊어지는 경우 라즈베리파이의 카메라 모듈 및 ESP32의           스태핑 모터, 서보 모터 구동 정지. 




