# CV_report

## c++でopencvを使うための環境構築
開発環境：Ubuntu
```
sudo apt update
sudo apt install libopencv-dev
sudo apt install cmake
sudo apt install g++
```

pythonでopencvを使うときはSpyder(anaconda3)上で実行している。

## ディレクトリ構成
- bilateral (bilateral filter用ファイル一式)
- guided (guided filter用ファイル一式)
- images (画像)
- non_local_means_blue (青色成分に対してnon-local meansを行うプロジェクト)
- non_local_means_green (緑色成分...)
- non_local_means_red (赤色成分...)
- non_local_means_merge (三色を統合)
- psnr (PSNR値を測定する)
- noise.py (ガウシアンノイズを乗せる)

## c++でopencv実行方法
1. 各プロジェクトフォルダへ行く
2. CMakeLists.txtを用意
3. 
```
mkdir build
cd build
cmake ..
make
./main "必要な引数"
```

## 各プロジェクト使用方法

### Bilateral filterでimages/Lenna_noise.jpgをフィルタリングしたいとき
```
cd bilateral
mkdir build
cd build
cmake ..
make
./main "../../images/Lenna_noise.jpg"
```
Guided filterも同様。

### Non-local means

各色のプロジェクトファイルで上と同様に実行した後、以下を実行して統合する。
```
cd non_local_means_merge
mkdir build
cd build
cmake ..
make
./main "../../non_local_means_blue/build/filtered_blue.png"
       "../../non_local_means_green/build/filtered_green.png"
       "../../non_local_means_red/build/filtered_red.png"
```

### PSNRの測定
引数に比較したい2画像のパスを与える。
```
cd psnr
mkdir build
cd build
cmake ..
make
./main <image1> <image2>
```