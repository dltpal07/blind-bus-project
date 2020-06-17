load('finmat.mat','point')  %c++에서 받아온 좌표 mat파일 경로로 바꾸기
point(1)=point(1);
point(2)=point(2);
point(3)=point(3);
point(4)=point(4);
width = point(4) - point(2);
height = point(3) - point(1);
%width = 52;
%height = 4;
A = imread('C:\Program Files (x86)\Intel RealSense SDK 2.0\samples\\x64\Debug\imageColor0.png'); %

I = imcrop(A, [point(2) point(1) width height]);  %작은x, 작은y, 넓이, 높이
I = imresize(I,[100, 260]);
figure,imshow(I);

I = rgb2gray(I);

Icorrected = imtophat(I,strel('disk',15));
marker = imerode(Icorrected, strel('line',10,0));
Iclean = imreconstruct(marker, Icorrected);
BW2 = imbinarize(Iclean);

results = ocr(BW2,'TextLayout','Block');


fin = results.Text
save("busmat.mat", "fin");