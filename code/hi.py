import sys
sys.path.append('C:\semi\py\\tensorflow_object_detection_helper_tool\object_detection')
import bus_test
def st(s):
    print("\n> please wait for object detection")
    loc = bus_test.bus_start(s)
    print(s)
    return loc

