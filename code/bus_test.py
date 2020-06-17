import numpy as np
import os
import six.moves.urllib as urllib
import sys
import tarfile
import tensorflow as tf
import zipfile
import shutil
from collections import defaultdict
from io import StringIO
from matplotlib import pyplot as plt
from PIL import Image

sys.path.append("..")
from object_detection.utils import ops as utils_ops

from utils import label_map_util

from utils import visualization_utils as vis_util
import PIL.ImageDraw as ImageDraw


def bus_start(search_name):
    print("\n ---------- start object detection by python ---------- \n")
    PATH_TO_CKPT = 'C:/semi/py/tensorflow_object_detection_helper_tool/export_dir/frozen_inference_graph.pb'

    # List of the strings that is used to add correct label for each box.
    PATH_TO_LABELS = 'C:/semi/py/tensorflow_object_detection_helper_tool/label_map.pbtxt'
    PATH_TO_RESULT = 'C:\\semi\\py\\tensorflow_object_detection_helper_tool\\object_detection\\result_picture'
    NUM_CLASSES = 5
    detection_graph = tf.Graph()
    with detection_graph.as_default():
      od_graph_def = tf.compat.v1.GraphDef()
      with tf.io.gfile.GFile(PATH_TO_CKPT, 'rb') as fid:
        serialized_graph = fid.read()
        od_graph_def.ParseFromString(serialized_graph)
        tf.import_graph_def(od_graph_def, name='')
    label_map = label_map_util.load_labelmap(PATH_TO_LABELS)
    categories = label_map_util.convert_label_map_to_categories(label_map, max_num_classes=NUM_CLASSES, use_display_name=True)
    category_index = label_map_util.create_category_index(categories)
    print('> importing library is finished\n')
    def load_image_into_numpy_array(image):
        (im_width, im_height) = image.size
        return np.array(image.getdata()).reshape(
            (im_height, im_width, 3)).astype(np.uint8)

    PATH_TO_TEST_IMAGES_DIR = 'C:\Program Files (x86)\Intel RealSense SDK 2.0\samples\\x64\Debug'
    #PATH_TO_TEST_IMAGES_DIR = 'C:\Program Files (x86)\Intel RealSense SDK 2.0\samples\save-to-disk'
    #PATH_TO_TEST_IMAGES_DIR = 'C:\semi\web\prj\flask'
    TEST_IMAGE_PATHS = [ os.path.join(PATH_TO_TEST_IMAGES_DIR, 'imageColor{}.png'.format(i)) for i in range(0, 1) ]
    depth_image_path = 'C:\Program Files (x86)\Intel RealSense SDK 2.0\samples\\x64\Debug\imageDepth0.png'
    # Size, in inches, of the output images.
    IMAGE_SIZE = (12, 8)

    def run_inference_for_single_image(image, graph):
      with graph.as_default():
        with tf.Session() as sess:
          # Get handles to input and output tensors
          ops = tf.get_default_graph().get_operations()
          all_tensor_names = {output.name for op in ops for output in op.outputs}
          tensor_dict = {}
          for key in [
              'num_detections', 'detection_boxes', 'detection_scores',
              'detection_classes', 'detection_masks'
          ]:
            tensor_name = key + ':0'
            if tensor_name in all_tensor_names:
              tensor_dict[key] = tf.get_default_graph().get_tensor_by_name(
                  tensor_name)
          if 'detection_masks' in tensor_dict:
            # The following processing is only for single image
            detection_boxes = tf.squeeze(tensor_dict['detection_boxes'], [0])
            detection_masks = tf.squeeze(tensor_dict['detection_masks'], [0])
            # Reframe is required to translate mask from box coordinates to image coordinates and fit the image size.
            real_num_detection = tf.cast(tensor_dict['num_detections'][0], tf.int32)
            detection_boxes = tf.slice(detection_boxes, [0, 0], [real_num_detection, -1])
            detection_masks = tf.slice(detection_masks, [0, 0, 0], [real_num_detection, -1, -1])
            detection_masks_reframed = utils_ops.reframe_box_masks_to_image_masks(
                detection_masks, detection_boxes, image.shape[0], image.shape[1])
            detection_masks_reframed = tf.cast(
                tf.greater(detection_masks_reframed, 0.5), tf.uint8)
            # Follow the convention by adding back the batch dimension
            tensor_dict['detection_masks'] = tf.expand_dims(
                detection_masks_reframed, 0)
          image_tensor = tf.get_default_graph().get_tensor_by_name('image_tensor:0')

          # Run inference
          output_dict = sess.run(tensor_dict,
                                 feed_dict={image_tensor: np.expand_dims(image, 0)})

          # all outputs are float32 numpy arrays, so convert types as appropriate
          output_dict['num_detections'] = int(output_dict['num_detections'][0])
          output_dict['detection_classes'] = output_dict[
              'detection_classes'][0].astype(np.uint8)
          output_dict['detection_boxes'] = output_dict['detection_boxes'][0]
          output_dict['detection_scores'] = output_dict['detection_scores'][0]
          if 'detection_masks' in output_dict:
            output_dict['detection_masks'] = output_dict['detection_masks'][0]
      return output_dict


    for image_path in TEST_IMAGE_PATHS:
      shutil.copyfile(image_path, os.path.join(PATH_TO_RESULT, str(len(os.listdir(PATH_TO_RESULT))))+'.png')
      shutil.copyfile(depth_image_path, os.path.join(PATH_TO_RESULT, str(len(os.listdir(PATH_TO_RESULT))))+'.png')
      image = Image.open(image_path)
      # the array based representation of the image will be used later in order to prepare the
      # result image with boxes and labels on it.
      image_np = load_image_into_numpy_array(image)
      # Expand dimensions since the model expects images to have shape: [1, None, None, 3]
      image_np_expanded = np.expand_dims(image_np, axis=0)
      # Actual detection.
      output_dict = run_inference_for_single_image(image_np, detection_graph)
      # Visualization of the results of a detection.
      im, loc, dis_str = vis_util.visualize_boxes_and_labels_on_image_array(
          image_np,
          output_dict['detection_boxes'],
          output_dict['detection_classes'],
          output_dict['detection_scores'],
          category_index,
          instance_masks=output_dict.get('detection_masks'),
          use_normalized_coordinates=True,
          line_thickness=8)
      plt.figure(figsize=IMAGE_SIZE)
      return_loc = []
      for i in range(len(dis_str)):
          if(dis_str[i] == search_name):
              return_loc = loc[i]
              return_loc.append(search_name)
              break
      if(len(return_loc) == 0):
          print("> object detection is failed")
          return_loc = [0,0,0,0]
          return_loc.append('fail')
    print("\n ---------- finish object detection by python ---------- \n")
    return return_loc
