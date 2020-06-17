# 2020-Capstone-Design

![캡스톤이미지](https://user-images.githubusercontent.com/44723287/84027141-7bdfb300-a9c9-11ea-978e-0625cd3c90bd.JPG)



## object detection
5taku의 tensorflow_object_detection_helper_tool을 사용하였다.
(https://github.com/5taku/tensorflow_object_detection_helper_tool)
원래의 tensorflow_object_detection_helper_tool의 object_detection 폴더에 
bus_test.py 파일을 추가하였고, object_detection의 utils의 visualization_utils를 code의
visualization_utils로 수정하였다.

## depth camera

intel에서 제공해주는 샘플 코드를 사용하였다.
(https://www.intelrealsense.com/sdk-2/)
intel RealSense SDK 2.0의 samples의 save-to-disk를 code의 save-to-disk 폴더로 수정하였다.
(python.h 헤더를 사용하기 위하여 visual studio 환경 설정 필요 - https://velog.io/@dltpal07/visual-studio-c%EC%97%90%EC%84%9C-python-%ED%98%B8%EC%B6%9C%ED%95%98%EA%B8%B0)

