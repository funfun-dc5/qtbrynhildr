
**************************************************************************
正式なマニュアルは整備中です。しばらくはこのReadMe.txtも参照ください。
**************************************************************************

----------------------------------------------------------------------
 Qt Brynhildr について
----------------------------------------------------------------------
==============================
 [最初に]
==============================
 このソフトウェアは仕様の公開されているパブリックモードで動作させた
 Brynhildrサーバに接続するためのクライアントソフトウェアです。

 今の所サーバ機能は持っていませんので、サーバとしてはBrynhildr(最新を推奨)を
 ご利用ください。

 ファイルの送受信、クリップボードの共有を行うにはパブリックモード6をサポートした
 Brynhildr2が必要になります。古いサーバについて、これらの機能が必要な場合はBrynhildrの
 クライアントモードをご利用ください。

 (*1) Brynhildrの標準のクライアントモードに対して大きなアドバンテージのある
      クライアントではありませんのでご承知おきください。
      このソフトウェアはより多くの環境(OS)でBrynhildrのリモートコントロール機能を
      利用できるようにすることを主たる目的としています。(あと作者の勉強のため…)

      また、パブリックモードでは通常のモードに比べてセキュリティレベルが低下します。
      重要な操作を行うような場合は通信路の暗号化などの対策を検討したほうがよいかも
      しれません。

 (*2) このドキュメントはすでに最新のBrynhildr(Ver.2.x.x)によるリモートデスクトップ環境を
      構築済みの方の利用を想定しています。

==============================
 [対応OS]
==============================

[Windows版(64bit/32bit)]

 Windows XP以降 (*3)

[Linux版(x64/x86)]

 Ubuntu 14.04 LTS以降 (x64/x86)
 CentOS 7 (x64)以降
 Fedora 33 (x64)以降

(*)今の所X.org推奨

[Cygwin x64] : 2021.2.25

 3.1.7 (0.340/5/3)以降 (*5)

[Raspberry Pi 3] : 2021.3.15

 Debian with Raspberry Pi Desktop (2021-01-11)以降
 (32bit:armv7l)

[Mac版]

 Mac OS X 10.10(Yosemite)以降

[FreeBSD x64] : 2021.2.26

 12.2-RELEASE以降

[Android版]

 Android 5.0以降 (*4)

 (*1) 全てのエディションで試したわけではありません。

 (*2) Qt5のサポートする環境(OS)で動作させられたらいいなぁと思っていますが、
      お約束は出来ません。

      Qt5のサポートする環境(OS)について詳しくは以下のページを御覧ください。
      https://doc.qt.io/QtSupportedPlatforms/index.html

 (*3) XP/Vistaで動作する版をビルドするにはバージョンが5.6.3以下のQt5を使って
      ビルドする必要があります。

 (*4) 全てのデバイスで試したわけではありません。Qt5のサポートする環境に依存します。

 (*5) パッケージとして配布されているQt5.9.4でビルドしたもので確認(要pulseaudioパッケージ)

==============================
 [テスト中の環境]
==============================
[iOS版]

 iOS 13以降

----------------------------------------------------------------------
1-1. インストール (Windows版)
----------------------------------------------------------------------
==============================
[インストール]
==============================

 インストーラを実行してインストールしてください。設定はデフォルトのままを
 お勧めします。

==============================
[アンインストール]
==============================

「コントロールパネル」->「プログラムと機能」からアンインストールしてください。
 インストール/作成されたすべてのファイルが削除されます。レジストリは使用しません。

----------------------------------------------------------------------
1-2. インストール (Linux版)
----------------------------------------------------------------------
====================================
[インストール]
====================================

 (1) Qt5をインストールします。(すでにインストール済みの場合はスキップしてください)

 環境に合わせて以下からダウンロードします。

 https://www1.qt.io/developers/

 root権限で実行してください、インストーラが表示されます。

 例)

 chmod 755 ./qt-opensource-linux-x64-5.12.9.run
 sudo ./qt-opensource-linux-x64-5.12.9.run

 (注) 上記をインストールしなくても起動することがありますが、メニューの場所が異なったり、
      文字化けが発生する可能性があります。

 (2) 自己解凍ファイルを実行してパッケージを展開します。

 例)

  chmod 500 ./QtBrynhildr-linux_x64-v210.run
  ./QtBrynhildr-linux_x64-v210.run

 (3) 起動確認します。

 [64bit]
 ./linux_x64/bin/Qt Brynhildr

 [32bit]
 ./linux_x86/bin/Qt Brynhildr

 (4) 起動して、エラーが出たら、必要なパッケージをインストールして、3)へ戻ります。
     ウィンドウが表示されたらすでに必要なパッケージはインストールされています。

  例えばUbuntuの場合libglu1-mesaなどが必要かもしれません。以下のように
  インストールしてください。

  sudo apt-get install libglu1-mesa

  [エラーメッセージの例]
  QtBrynhildr: error while loading shared libraries: libGL.so.1: cannot open shared object file: No such file or directory

  [CentOSの場合]

  EPELリポジトリを追加(sudo yum install epel-release)して以下のパッケージを
  インストールしてください。

  sudo yum install libqtxdg-qt5
  sudo yum install qt5-qtmultimedia

==============================
[インストール] (Ubuntu版)
==============================

 (1) root権限でQt5パッケージをインストールします。
     (すでにインストール済みの場合はスキップしてください)

   sudo apt-get install qt5-default libqt5multimedia5-plugins

 (2) 自己解凍ファイルを実行してパッケージを展開します。

 例)

  chmod 500 ./QtBrynhildr-ubuntu_x64-v210.run
  ./QtBrynhildr-ubuntu_x64-v210.run

 (3) 以降はLinux版と同じ

==============================
[アンインストール]
==============================

 (1) 以下を削除してください。

  ~/.config/mcz-xoxoとlinux_x64/linux_x86以下

 (2) 気になる人はインストールした以下のパッケージもアンインストールしてください。

  (*) Qt5のアンインストール

     初期設定のままインストールすると"/opt/Qt5.x.x"の下に"MaintenanceTool"
     というプログラムがインストールされます。このプログラムを起動すればGUIで
     アンインストールを実行できます。

  (*) その他のパッケージのアンインストール

  [Ubuntuの場合]

  sudo apt-get remove libglu1-mesa
  sudo apt-get autoremove

----------------------------------------------------------------------
1-3. インストール (Cygwin(x64)版)
----------------------------------------------------------------------
====================================
[インストール]
====================================

 (1) 実行に必要なパッケージをインストールします。

    Qt5, pulseaudio

 (2) アーカイブファイルを展開します。

  $ tar xvfJ QtBrynhildr-cygwin_x64-v233.tar.xz

====================================
[アンインストール]
====================================

----------------------------------------------------------------------
1-4. インストール (Mac版)
----------------------------------------------------------------------
==============================
[インストール]
==============================

 .dmgファイルをマウントし、Qt Brynhildr.app を /Applications にコピーしてください。

==============================
[アンインストール]
==============================

 ~/.config/mcz-xoxoと/Applications/Qt Brynhildr.app を削除してください。

----------------------------------------------------------------------
1-5. インストール (FreeBSD(x64)版)
----------------------------------------------------------------------
====================================
[インストール]
====================================

====================================
[アンインストール]
====================================

----------------------------------------------------------------------
1-6. インストール (Android版)
----------------------------------------------------------------------

[野良アプリ版]

QtBrynhildr-android-v210.apk [32bit/64bit同梱版]

[Google Play版]

"Qt Brynhildr"で検索インストール

=========================================================
[タッチパネル操作]
=========================================================

---------------------------------------------------------
[サーバに接続ダイアログ表示中]
---------------------------------------------------------

[1点ダブルタップ] 接続に成功したサーバホストのIP/ホスト名履歴を呼び出す

---------------------------------------------------------
[通常画面]
---------------------------------------------------------
=======================================
[Qt Brynhildrオペレーションタイプの時]
=======================================

[1点タップ] マウスカーソル移動 + マウス左ボタン・クリック

[ピンチ・イン] デスクトップ画面縮小

[ピンチ・アウト] デスクトップ画面拡大

[1点ドラッグ(全画面時)] マウスカーソル移動

[1点ドラッグ(拡大時)] 表示領域スクロール

[2点タップ] マウス右ボタン・クリック

[3点タップ] Windowsキーの送出

=========================================
[KeroRemote互換オペレーションタイプの時]
=========================================

[1点タップ(全画面時)] マウスカーソル移動 + マウス左ボタン・クリック

[1点タップ(拡大時:0.5s以内)] マウスカーソル移動 + マウス左ボタン・クリック

[ピンチ・イン] デスクトップ画面縮小

[ピンチ・アウト] デスクトップ画面拡大

[1点ドラッグ(全画面時)] マウスカーソル移動 + マウス左ボタン + ドラッグ

[1点ドラッグ(拡大時)] 表示領域スクロール

[3点タップ] Windowsキーの送出

---------------------------------------------------------
[ソフトウェア・キーボード/ボタン表示]
---------------------------------------------------------

(1) タッチパネル・インターフェースが左/右タイプの時(初期値)

[画面左辺の外から中央に向かってドラッグ(長めに)] ソフトウェア・ボタン表示

[画面右辺の外から中央に向かってドラッグ(長めに)] ソフトウェア・キーボード表示

(2) タッチパネル・インターフェースが上/下タイプの時

[画面上辺の外から中央に向かってドラッグ(長めに)] ソフトウェア・ボタン表示

[画面下辺の外から中央に向かってドラッグ(長めに)] ソフトウェア・キーボード表示

---------------------------------------------------------
[ソフトウェア・キーボード表示中]
---------------------------------------------------------

[Close] ソフトウェア・キーボードを非表示

[Shift/Ctrl/Alt/Fn] タップすると有効/無効がトグルで切り替わる

[1点タップ(キーボード領域以外)] マウスカーソルの移動

---------------------------------------------------------
[ソフトウェア・ボタン表示中]
---------------------------------------------------------

[Close] ソフトウェア・ボタンを非表示

[FPS/データ転送量] 情報表示のみ

[Left]  マウス左ボタン

[Right] マウス右ボタン

[Wheel+] ホイールを上に回転

[Wheel-] ホイールを下に回転

[1点ドラッグ(ボタン以外の領域)] マウスカーソルの相対移動

[Option] オプション系ボタンの表示/非表示

[Monitor] モニタの選択

[Sound] サウンドのON/OFF

[Sound Cache] キャッシュレベルの選択

[Sound Quality] サウンド音質の選択

[Video Codec] ビデオコーデックの選択

[Video Quality] ビデオ画質の選択

[Video FPS] ビデオFPSの選択

---------------------------------------------------------
[追加されたメニュー」
---------------------------------------------------------
(1)「デコードオプション」

   「タイプ」 -> 「C++」/「NEON」

     モード7のデコーダのタイプを変更します。「NEON」ではSIMD命令であるNEONを利用します。
     標準では「C++」です。

   「スレッド」 -> 「1スレッド」/「2スレッド」/「4スレッド」

     モード7のデコーダのスレッド数を変更します。標準では2スレッドです。

(2) 「タッチパネル・オペレーション」

   「Qt Brynhildrタイプ」(初期値)

     Qt Brynhbildrのタッチパネル操作

   「KeroRemoteタイプ」

     KeroRemote互換のタッチパネル操作

(3) 「タッチパネル・インターフェース」

   「左/右タイプ」(初期値)

     左辺(Left Side)でSoftware Button、右辺(Right Side)でSoftware Keyboardを表示

   「上/下タイプ」

     上辺(Top Side)でSoftware Button、下辺(Bottom Side)でSoftware Keyboardを表示

----------------------------------------------------------------------
1-7. インストール (iOS版)
----------------------------------------------------------------------

----------------------------------------------------------------------
2. Brynhildrサーバの準備 (パブリックモードの有効化)
----------------------------------------------------------------------

**********************************************************************
(注) Brynhildr 1.1.0でパブリックモードは標準で有効となりました。
     ただし、1.0.7以前からお使いの場合は以下の(3)の確認が必要です。
     Brynhildr 1.1.0以降を0から導入した場合はすでに有効になっています。
**********************************************************************

 (1) サーバを実行中の場合は一旦サーバを終了させてください。

 (2) brynhildr.exeと同じフォルダーにあるbrynhildr.iniをエディタで開きます。

 (3) [ServerMode]セクションにある以下の設定を変更して保存します。

   publicmode=0 -> publicmode=1

 (4) brynhildr.exeをサーバモードで再び起動します。
     これでパブリックモードが有効となります。

 (*1) 確認のためBrynhildrサーバのログを出力させておくことをお勧めします。
      brynhildr.exeと同じフォルダにlog.txtというファイルを作成しておけば
      出力されるそうです。

----------------------------------------------------------------------
3. Qt Brynhildr の動作確認
----------------------------------------------------------------------
==============================
 サーバへの接続
==============================

 (1) Qt Brynhildrを起動します。

 (2)「ファイル」メニューから「接続」を選んで、「サーバに接続」ダイアログを
     表示してください。

 (3) 必要な情報を入力してください。

   [1] 「ホスト名/IPアドレス」

      サーバ名もしくはIPアドレスを指定してください。

   [2] 「ホストタイプ」

      サーバOSを選択してください。

   [3] 「キーボードタイプ」

      サーバのキーボードタイプ(JP Keyboard(日本語キーボード)もしくは
      US Keyboard(英語キーボード))を選択してください。
      (英語キーボードはテストが十分ではありません…)

      Windows版のみ"Native Keyboard"を選択できます。サーバとクライアントのキーボードタイプが
      同じである必要がありますが、日本語および英語以外のサポートしていないキーボードを
      利用できます。

   [4] 「ポート番号」

      ポート番号を指定してください。

   [5] 「パスワード」

      パスワードを入力してください。(現在のところパスワードは終了時に保存されません)
      パブリックモードではパスワードは最長半角16文字までです。

   [6] 「パブリックモード」

      パブリックモードのバージョンを選択してください。

   [7] 「パスワードを表示」

      チェックを入れると入力中のパスワードを平文で表示するようになります。

   [8] 「フルスクリーン」

      チェックを入れるとフルスクリーンでサーバに接続します。

   [9] 「ビューワーモード」

      チェックを入れるとビューワーモードでサーバに接続します。

  (4) 「OK」ボタンを押してサーバに接続してください。

==============================
 サーバからの切断
==============================

 (1) 終了する時は「ファイル」メニューから「終了」を選ぶか、ウィンドウの
     「X」ボタンを押してください。
     一時的にサーバから切断する場合は「ファイル」メニューから「切断」を選んでください。

 (2) 「確認」ダイアログが出るので、終了して良い場合には「OK」ボタンを押して
      Qt Brynhildrを終了してください。

      「次回も確認ダイアログを表示する」のチェックを外すと次回から「確認」ダイアログを
      表示せずにすぐに終了します。

==============================
 使い方
==============================

 (1) 「ファイル」メニュー

     [1] 接続

     サーバに接続するための「サーバに接続」ダイアログを表示します。

     [2] 切断

     サーバから切断します。

     [3] ファイルを送信

     ファイルをサーバに送信します。送信するファイルを選択するために「ファイル選択」
     ダイアログが表示されます。

     [4] ファイルの転送を中止

     ファイルの転送(送受信)を中止します。一旦サーバを切断し、再接続します。

     [5] 設定

     「設定」ダイアログを表示します。

     [6] 初期化

      設定を初期化します。.iniファイルも初期値に戻します。

     [7] 終了

      Qt Brynhildr を終了します。 「確認」ダイアログが表示されるので「OK」ボタンを押すと
      終了します。

 (2) 「表示」メニュー

     [1] ツールバーを表示

     ツールバーの表示のON/OFFを変更します。

     [2] ステータスバーを表示

     ステータスバーの表示のON/OFFを変更します。

     [3] フレームレートを表示

     フレームレート(FPSおよびデータレート)の表示のON/OFFを変更します。

     [4] デスクトップのスケールを固定

     デスクトップ画像のスケールを変更不可にします。

     [5] ウィンドウサイズを固定

     ウィンドウサイズを変更不可にします。

     [6] 常に最前面に表示

     常に最前面に表示します。

     [7] フルスクリーン

     フルスクリーンにします。ESCキーで通常のウィンドウモードに戻ります。

 (3) 「ビデオ」メニュー

     [1] 画質選択

     デスクトップ画像の画質を変更します。画質を良くするとデスクトップ画面の
     更新が粗くなり、画質を低くすると比較的デスクトップ画面の更新がスムーズに
     なります。

     [2] フレームレート選択

     画面書き換えの頻度(FPS)の最大値(maxfps)を変更します。

     [3] デスクトップのスケーリング

     デスクトップ画像のスケールを変更するダイアログを表示します。
     スライダーにより変更して「OK」ボタンを押すと有効になります。
     スライダーは左右のカーソルキーでも変更できます。
     「キャンセル」ボタンを押すと変更が無効になり、変更前のサイズとなります。
     「リセット」ボタンを押すと等倍サイズに戻ります。


     [4] デスクトップのキャプチャ

     デスクトップ画像をキャプチャします。保存される画像はスケーリング後の
     サイズになります。

     ホームディレクトリに"Desktopshot-YYYY-MM-DD-hh-mm-ss.jpg"のフォーマットで
     保存されます。保存場所/フォーマットは"Qt Brynhildr.ini"を書き換えることにより変更できます。
     対応フォーマットは.bmp/.jpg/.png/.ppm/.xpmなどです。

     ".jpg"でなく"jpg"と指定するとスケーリング前のオリジナルサイズで保存されます。

 (4) 「サウンド」メニュー

     [1] 音質選択

     サウンドの音質を変更します。サンプリング周波数を変更することで転送する
     データ量を減らすことが出来ます。音質が変わらない場合もあります。

     [2] サウンドキャッシュ

     サウンドデータのキャッシュレベルを変更します。

 (5) 「コントロール」メニュー

     [1] キーを送信

         以下の特殊キー（の組み合わせ）をサーバに送ります。

         (1) Alt キー + F4 キー

         (2) Ctrl キー + Esc キー

         (3) Windows キー

         (4) PrintScreen キー

         (5) Alt キー + PrintScreen キー

         (6) Ctrl (ON/OFF) キー

     [2] モニタ選択

         マルチモニタを利用している場合に操作対象とするモニタを変更します。
         モニタ1(メインモニタ)からモニタ9を選択します。
         1台しかモニタを接続していない場合は変更できません。
         「モニタ すべて」を選択するとサーバに接続しているすべてのモニタ画面を同時に表示します。

     [3] コントロール ON/OFF

         入力デバイス情報転送のON/OFFを変更します。OFFにするとキーボード/マウスの
         動きがサーバに送られません。
         サーバ画面の監視だけをしたい時にOFFにすればよいかもしれません。

     [4] グラフィックス ON/OFF

         デスクトップ画像転送のON/OFFを変更します。OFFにするとデスクトップ画像が
         更新されません。デスクトップ画像データの転送量を減少させます。
         OFFにするとコントロールも同時にOFFとなります。

     [5] サウンド ON/OFF

         サウンド情報転送のON/OFFを変更します。OFFにするとサウンド情報が
         転送されないため消音状態になります。

     [6] パブリックモード選択

         パブリックモードを変更します。
         モード5からモード7のいずれかを選択します。

     [7] 記録と再生 (*)

         (1) 記録開始

           リモート操作の記録を開始します。

         (2) 記録終了

           リモート操作の記録を終了します。ファイルダイアログにより保存するファイルを
           指定します。

         (3) 再生

           リモート操作の再生を開始します。ファイルダイアログにより再生するファイルを
           指定します。

      (*) デスクトップ上の絶対座標でマウス位置を記録しているため操作するウィンドウの
          位置・サイズに依存するような操作は正しく再生できません。例えば、起動の度に
          ウィンドウ位置が変わったりすると押したいボタンが押せずにその後の操作が意味のない
          操作になる可能性があります。
          また、サーバの負荷によってウィンドウの表示タイミングなどがずれることで同様の現象
          となる可能性があります。

     [8] プラグインの無効化

         サーバ上のプラグインを無効化します。再度有効化するためには再接続が必要です。

 (6) 「オプション」メニュー

     [1] デスクトップ圧縮モード

         一時的にデスクトップを縮小(1/2, 1/4, 1/8)表示します。解除すると元のサイズに戻ります。

     [2] ビューワーモード

         デスクトップを観るだけのモードで、キーボード、マウス操作はできません。
         解除すると通常のモードに戻ります。
         この時左ボタンドラッグで観たいエリアを指定できます。右ボタンクリックで元の状態に戻ります。

     [3]  ふるふるモード

        マウスを少しずつ動かします。スリープやスクリーンセーバーの起動を抑止できます。

     [4] スクロールモード

         ウィンドウサイズを固定とした場合にスクロールバー表示のON/OFFを変更します。

     [5] モノクロモード

         モノクロモードのON/OFFを変更します。

     [6] マウストラッキングモード

         常にマウス移動を送信するマウストラッキングモードのON/OFFを変更します。

     [7] キーボードログ出力

         キーボードログ出力のON/OFFを変更します。

     [8] デバッグログ出力

         デバッグログ出力のON/OFFを変更します。デバッグ用の機能で将来残るかは
         未定です。

     [9] テスト中の機能

         テスト中の機能がない場合メニュー自体存在しない場合があります。

         現在テスト中の機能は以下です。

         **現在上記以外のテスト中の機能はありません**

(7) 「ヘルプ」メニュー

     [1] ログの表示

       Qt Brynhildr のログを表示します。

     [2] アップデートを確認

       新しいバージョンがリリースされていないかを確認します。

     [3] ヘルプ ブラウザ (デスクトップ版のみ)

       「はじめてのQt Brynhildr」を簡易ブラウザで開きます。

     [4] Qt Brynhildr について

       Qt Brynhildr についてバージョン情報などを表示します。

==================================
 ファイル転送とクリップボード共有
==================================

新たに公開されたパブリックモード6/7を利用して接続するとファイル転送機能と
クリップボード共有機能を使うことが出来ます。(*)

 (1) ファイル転送

 サーバ(Brynhildr)からクライアント(Qt Brynhildr)へ、クライアントからサーバへ
 ファイルを転送できます。

  [1] サーバからクライアントへのファイル転送

  Brynhildr2のアイコンからメニューを表示させ「SendFile」を選択し、転送したい
  ファイルを指定します。初期設定ではQt Brynhildrはユーザーディレクトリの
  直下にファイルを保存します。

  [2] クライアントからサーバへのファイル転送

  「ファイル」メニューから「ファイルを転送」を選択し、転送したいファイルを
  指定します。初期設定ではBrynhildr(brynhildr.exe)のあるフォルダの下に
  作成される"recv"フォルダ下に保存されます。

  (*1) ファイル転送中キーボード、マウス操作は出来ません。

  (*2) ドラッグ&ドロップによるファイル転送もサポートしています。
       設定ファイルを以下のように変更することで無効化出来ます。

       onTransferFileByDragAndDrop=false

 (2) クリップボード共有

 サーバ、クライアントのクリップボード（テキストのみ)を共有できます。
 サーバ側でクリップボードを更新するとクライアント側のマシンの
 クリップボードも同様に更新されます。逆も同様です。


 (*) パブリックモード6でうまく動作しない場合は設定ファイルを
     以下のように変更することで従来の動作となります。

        publicModeVersion=5

     この場合ファイル転送、クリップボード共有機能は利用できません。

----------------------------------------------------------------------
4. Qt Brynhildr の設定の保存場所 (内部資料)
----------------------------------------------------------------------

アプリケーション用の設定は終了時に保存され、次回起動時に読み込まれます。
保存場所はプラットフォームによって異なります。

[Windows版]
ユーザフォルダの以下のファイルに保存されます。

C:\Users\ユーザ名\AppData\Roaming\mcz-xoxo\Qt Brynhildr.ini

[UNIX系OS版]
ホームディレクトリの以下のファイルに保存されます。

~/.config/mcz-xoxo/Qt Brynhildr.ini

(*) 起動時に引数としてファイル名を渡すことで、任意のファイルを
    指定できます。「5. オプション」を参照してください。

============================================================
[内容の例]
============================================================

インタフェース検討中の設定はこれらの値を直接変更すれば変更可能です。
(*)内部保存情報なので直接変更は推奨しません。

(注意)不正な値を設定すると起動しなくなる可能性があります。
      その場合はファイルを削除するか、-initオプションを指定して再起動すれば
      初期状態に戻ります。

      ファイルをコピーしておけば設定をバックアップできます。

------------------- ここから -------------------
[General]
infoKernelType=winnt
infoKernelVersion=10.0.19042
infoProductType=windows
infoProductVersion=10
infoBuildAbi=x86_64-little_endian-llp64
generatedVersion=2203
currentVersion=2204
publicModeVersion=7
serverName=mcz.world.idol.jp
serverType=5
keyboardType=0
keyboardTypeName=JP Keyboard (built-in)
portNo=55500
onControl=true
onTaskbarAssist=true
taskbarAssistAreaWidth=10
taskbarAssistAreaHeight=10
onPluginsDisable=false
onNativeMenuBarDisable=false
onSoundCriticalMessageDisable=false
onHoldMouseControl=false
onExtraButtonSupport=true
onControlOffWithGraphicsOff=true
onSendControlKeyState=false
onGraphics=true
videoQuality=5
onGamePadSupport=true
onBrynhildr2Support=true
frameRate=30
onSound=true
soundType=1
soundQuality=3
soundCacheTime=100
desktopScalingFactor=1
desktopScalingQuality=1
desktopScalingType=0
desktopCompressMode=1
onCutDesktopBlankArea=false
onMonochromeMode=false
onMouseTrackingMode=true
desktopOffsetX=0
desktopOffsetY=0
monitorNo=1
onOpenConnectToServerDialogAtBootup=true
onCheckUpdateAtBootup=false
onConfirmAtExit=true
onSaveSettingsAtExit=false
onExitAfterReplay=false
onStaysOnTop=false
onDesktopScaleFixed=false
onWindowSizeFixed=false
onDesktopAutoresize=true
onFrameLessWindow=false
onShowMenuBar=true
onShowStatusBar=true
displayDataCounterType=1
onFullScreenAtConnected=false
onViewerAtConnected=false
onHideMenuAndStatusBarAtFullScreen=true
onShowFrameRate=true
onShowPassword=false
onClipCursor=false
onShowMouseCursorMarker=false
onTransferFile=true
SIMDOperationTypeName=C++
onShowTotalProgressForTransferFile=false
onTransferFileByDragAndDrop=true
onTransferClipboard=true
graphicsBufferSize=1048576
soundBufferSize=1048576
doubleClickThreshold=500
convertThreadCount=2
timeoutTime=-1
touchpanelOperationType=0
touchpanelInterfaceType=0
outputGraphicsDataToFile=false
outputSoundDataToFile=false
outputSoundDataToWavFile=false
outputLog=false
outputPath=C:\\Users\\funfun\\
keylayoutPath=C:\\Users\\funfun\\AppData\\Roaming\\mcz-xoxo\\keylayout
logFile=D:\\tmp\\qtbrynhildr.log
keyboardLogFile=D:\\tmp\\keyboard.log
desktopCaptureFormat=.jpg
serverNameListSize=10
serverName0=mcz.world.idol.jp
serverName1=funfun-PC
serverName2=192.168.253.128
serverName3=
serverName4=
serverName5=
serverName6=
serverName7=
serverName8=
serverName9=
windowPos=@Point(200 200)
windowSize=@Size(800 600)
windowState=@ByteArray(xxxxx)
------------------- ここまで -------------------

----------------------------------------------------------------------
5. オプション
----------------------------------------------------------------------

 (0) -mode5/mode6/mode7オプション

 パブリックモードのバージョンを指定します。無指定時は-mode7指定相当となります。

 -mode5 : ファイル転送、クリップボード共有をサポートしません。古いサーバに接続するために
          指定します。
 -mode6 : ファイル転送、クリップボード共有をサポートします。

 -mode7 : mode6に加えてVP8によるデスクトップ画像転送を行います。
          mode5,6に比べて画像データの転送データ量が少なくなります。

 (1) -serverオプション

 -server <server name|IP address>:<password>[:<port no>[:<host type>]]

 サーバを指定して、起動後すぐに接続します。
 (*)キーボードタイプは指定できません。.iniの設定をそのまま使います。

 host typeは、xp/vista/7/8/8.1/10 を指定します。

 (2) -celtオプション

 -celt

 CELTによりサウンドデータを圧縮して転送します。

 (3) -recordオプション

 -record <file>

 リモート操作情報をファイルに保存します。-replayを同時に指定した場合無効となります。

 (4) -replayオプション

 -replay <file>

 リモート操作情報をファイルから読み込み、操作を再現します。

 (5) -fullscreenオプション

 -fullscreen

 フルスクリーンで接続します。

 (6) -viewerオプション

 -viewer

 ビューワーモードで接続します。

 (7) -version(-v)オプション

 -version (-v)

 バージョン情報を表示します。

 (8) -specオプション

 ビルド情報を表示します。

 (9) -help(-h)オプション

 -help (-h)

 ヘルプメッセージを表示します。

 (10) -initオプション

 すべての設定を初期値に戻します。

 (11) -notransオプション

 メニューテキスト、メッセージなどの翻訳を抑止し、すべて英語表記となります。

 (12) <.ini file>

 使用する設定ファイルを指定します。拡張子は".ini"である必要はありません。
 存在しないファイルを指定した場合は、新たに作成します。

----------------------------------------------------------------------
6. 注意
----------------------------------------------------------------------

Windows版ではランタイムライブラリパッケージが必要な場合があります。

以下のサイトなどから入手できます。

 * Microsoft <https://support.microsoft.com/ja-jp/help/2977003/the-latest-supported-visual-c-downloads>

 * 窓の杜 <https://forest.watch.impress.co.jp/>

----------------------------------------------------------------------
7. メッセージダイアログについて
----------------------------------------------------------------------

 (1) 「サーバのサウンドデバイスをチェックしてください！(Please check server sound device!)」

 サーバのサウンド設定の変更が必要かもしれません。
 サーバのサウンドの再生デバイスプロパティのスピーカー周波数を

 「16ビット、44100Hz(CDの音質)」

 に変更すれば改善されるかもしれません。

 (2) 「サウンドデバイスが見つかりません!(Not found sound device!)」

 サウンドを再生するためのデバイスが見つかりません。
 サウンド再生が正しくできることを確認してください。サウンド再生は強制的にOFFとなります。

 (3) 「サーバとの接続に失敗しました！(Failed to connect server!)」

 サーバに接続できませんでした。
 サーバ名もしくはIPアドレスが間違えているかもしれません。

 (4) 「パスワードが違います！(Password Error!)」

 パスワードが間違えているかもしれません。

 (5) 「すでに他のクライアントが接続しています！(Already connected another client!)」

 すでにサーバに他のクライアントが接続しているかもしれません。

 (6) 「パブリックモードでサポートしていないビデオコーデックです!(Not support Video Codec!)」

 パブリックモードでサポートしていないビデオコーデックがサーバで指定されています。(*)

 brynhildr.iniでvideocodec=2(MotionJPEG)もしくはvideocodec=3(Compress)と指定していること
 もしくはvideocodec自体の指定がないことを確認してください。

 (*) 現在のところvideocodec=0(Speed)/1(Hybrid)はサポートしていません。

 (7) 「サーバとの接続にタイムアウトしました！(Timeout to connect server!)」

 "Qt Brynhildr.ini"のtimeoutTimeで設定されている時間だけ待ちましたがサーバに
 接続できませんでした。サーバ名もしくはIPアドレスが間違えているかもしれません。

----------------------------------------------------------------------
8-1. 制限
----------------------------------------------------------------------

 (1) アスペクト比固定リサイズの調整機構が未実装です。

 (2) サーバ接続中にパブリックモード6からモード7に変更した場合マウスカーソル形状が
     しばらくの間正しく反映されない場合があります。

----------------------------------------------------------------------
8-2. Windows版のみの制限
----------------------------------------------------------------------

 (1)「ウィンドウサイズを固定」を選択した時、一時的にウィンドウが閉じるため
     画面がフラッシュしたように見えます。

----------------------------------------------------------------------
8-3. Linux版のみの制限
----------------------------------------------------------------------

 (1) クライアントのデスクトップ以上にスケールアップしてからスケールダウンすると
     ウィンドウがアップデートされずにデスクトップの残像が残ることがあります。

 (2) デスクトップのスケールアップが一定サイズ以上できないことがあります。(x86版のみ)
     カーネルパラメータ(shmmax)を変更することで回避できますが、root権限が必要です。

 (3) デスクトップの自動スケーリングが正しく行われない場合があります。

 (4) 起動時に端末に以下のメッセージが表示されることがあります。

    qt.network.ssl: QSslSocket: cannot resolve SSLv2_client_method
    qt.network.ssl: QSslSocket: cannot resolve SSLv2_server_method

    インストールされているOpenSSLのライブラリがSSLv2を無効にしてビルドされているためです。

    以下のページが参考になります。

    https://askubuntu.com/questions/711863/installing-qt-5-5-1-on-ubuntu-14-04-throws-qt-network-ssl-error-on-terminal

 (5) マウストラッキングモードが正常に動作しません。

     常にONの状態となります。

----------------------------------------------------------------------
8-4. Mac版のみの制限
----------------------------------------------------------------------

 (1) 端末から起動した場合、かなキーが反映され、端末上でIMがONになることがあります。

 (2) コントロールキーを押しながらのマウス左ボタンクリック操作が正しく扱えません。

     Control+クリックは副ボタンに割り当てられるためです。

     「コントロール」メニュー -> 「キーを送信」 -> 「Ctrl (ON/OFF)」を利用して
     コントロールキーの押下状態を実現できます。

 (3) 「フルスクリーンにする」、「検索」メニュー項目が存在します。

     システムが標準で用意している同名の項目が有効になっているためです。

 (4) スクロールモードが有効になりません。

     調査中です。

 (5) マウストラッキングモードが正常に動作しません。

     常にOFFの状態となります。

----------------------------------------------------------------------
8-5. FAQ(よく聞かれる質問)
----------------------------------------------------------------------

 (1) アプリケーションをインストールしたり、起動しようとすると操作ができなくなることがあるのですが？

    * サーバ側でUAC(ユーザアカウント制御)のダイアログが表示されたりしていませんか？

      Brynhildrサーバを管理者権限で起動し直して、同時にサービスに登録すると回避できるかもしれません。
      ログイン画面を操作したい場合も同様です。また管理者権限でUACのレベルを下げることで回避できますが
      推奨されません。

 (2) サーバに接続できないのですが？

    * サーバのホスト名, IPアドレスは正しいですか？

      サーバのタスクバー内のBrynhildrのアイコンをクリックすればIPアドレスが確認できます。

    * サーバはパブリックモードを有効にして起動していますか？

      サーバのbrynhildr.iniの中で "publicmode=1" となってることを確認してください。

    * サーバのファイアウォールの設定は適切になっていますか？

    * セキュリティーソフトの設定は適切になっていますか?

    * ネットワーク的に到達可能に設定されていますか?

      例えば家にあるPCに会社のPCからアクセスしようとしていませんか?
      普通はできないようになっています。

 (3) Mac版で内蔵スピーカを利用した時にサウンドにプチノイズが入るのですが?

    * サーバのタスクマネージャーを開いてサーバの負荷を確認してください。

      サーバの負荷が高い場合にはサウンドデータの転送が間に合わない場合があります。
      サウンドデータをPCMで転送するように指定すると改善するかもしれません。

 (4) Mac版で怪しいアプリだと言われて起動できないのですが?

    * GateKeeperに怪しいアプリだと判断されています。

      現在のMac版はAppleから言わせれば野良アプリです。現在最新のmacOS Catalina(10.15.7)では
      通常そのままでは起動できません。

      正当なアプリだという証明がないため、GateKeeperという仕組みで「怪しいアプリだよーん」と
      言われて起動できないようになっています。しかし、裏技的に起動する方法がまだ存在して、
      リモートデスクトップを使おうとするようなレベルのユーザーなら、その方法も分かるだろう
      という想定のもと、野良アプリとして公開しているのが現状です。

      もしかしたら、次の最新のmacOSではこの方法が取れなくなる可能性がありますのでご承知おきください。

      正しくはApple Developerとして年会費1万円ちょっとを支払って、正当なアプリの証明書をつけてもらうのが
      よいのは分かっていますが、毎年年会費を払うのは結構たいへんなのです。

      で、今の所の対応方法ですが、Finderを起動して「アプリケーション」を表示させます。
      次に「Qt Brynhildr」を選択してポップアップメニューを表示させ、そこでcontrolキーを押しながら
      「開く」をクリックします。で、一度起動に失敗したら、「システム環境」を開き、次に「セキュリティーと
      プライバシー」を開きます。ここで「一般」タブを選択すると「ダウンロードしたアプリケーションの実行許可」
      のところで「Qt Brynhildrを信頼して開きますか？」というような設定が出ると思います。

      この方法が取れなくなったら、App Storeに有料で登録するしかなさそうですが、いくらにしたら
      年間一万円をペイできるのでしょうかね…

      また、Windows版もやっぱり野良アプリです。ほんとはパッケージが正しいものと判断するための
      電子署名が必要です。証明書をつけるためにはやっぱりお金が必要です…

 (5) 「タスクバーを自動的に隠す」をオンにしているとスケールが小さい時にタスクバーを
     出すことが難しいのですが?

     現在はウィンドウの外からウィンドウ内にゆっくりマウスカーソルを動かすとタスクバーが
     出やすいようにアシスト機能が入っています。以下のようにすればこれを無効化できます。

      onTaskbarAssist=false

 (6) 「Monochrome」機能はないのでしょうか?

     パブリックモードではMonochrome(モノクロモード)は正式にはサポートされていません。
     Ver.2.3.9で独自にモノクロ化する機能を追加しました。

 「このようなFAQ情報があるといいかも」など、お知らせ頂けると助かります。

----------------------------------------------------------------------
9. 不具合の報告について
----------------------------------------------------------------------

以下にお願いします。

ふんふん(FunFun) : fu.aba.dc5@gmail.com

迷惑メールと判断されないよう件名(Subject)の先頭には"[Qt Brynhildr]"を
付加するようお願いします。

件名の例)

[Qt Brynhildr] 全然動きませんけど…


提供して頂きたい情報は以下のような情報です。
分かる範囲でご記入ください。

(*) 情報共有のため、お知らせいただいたメールの内容を一部ブログで
    引用させていただくことがあるかもしれません。
    内容について非公開をご希望の方はその旨お知らせください。

(1) サーバマシン/クライアントマシンそれぞれについて

 [1] OS名

 [2] 64bit/32bit

 [3] サービスパックの適用(ServicePack/update)の有無 [Windows版のみ]

 [4] カーネルのバージョン [Linux版のみ]

 [5] 接続形態 (LAN/WAN)

 [6] メモリ容量

 [7] モニタの解像度と接続方法(D-Sub/DVI/HDMIなど)

 [8] 接続しているキーボード、マウス

(2) サーバについて

 [1] お使いのBrynhildrのバージョン

 [2] 不具合発生時のlog.txtの内容
    (brynhildr.exeと同じ場所に空の"log.txt"を作成する必要があります)

(3) Qt Brynhildrについて

 [1] お使いのQt Brynhildrのバージョン

 [2] 不具合発生時のqtbrynhildr.log/keyboard.logの内容

 [3] キーボードタイプ (JP/US/Native Keyboard)

(4) 不具合内容

(5) 不具合再現手順

  (*) 細かく手順を書いていただけると不具合修正が速やかに行なえます

----------------------------------------------------------------------
10. 変更履歴
----------------------------------------------------------------------

doc/ChangeLog.txtを参照してください

----------------------------------------------------------------------
11. 情報へのリンク
----------------------------------------------------------------------

(1) Brynhildrに関するページ

 [1] 原作者IchiGekiさんのブログ

 「リモートデスクトップエンジニアのブログ。」: http://blog.x-row.net/

 [2] Brynhildrのページ

 「Brynhildr - 高速度型リモートデスクトップ」: http://blog.x-row.net/?p=2455

 [3] KeroRemoteのページ

 「iOS版KeroRemoteからWindowsへの接続方法。」: http://blog.x-row.net/?p=6732

  Android版のインターフェース実装の参考にさせて頂きました。

 [4] KeroKeyboardのページ

 「KeroKeyboard - スクリーンキーボード」: http://blog.x-row.net/?p=14018

  ソフトウェア・キーボードのレイアウトの参考にさせて頂きました。

(2) パブリックモード、クライアントサンプルソースに関するページ

 パブリックモードに関する記事があります。
 また、Visual StudioでビルドできるWindows向けのクライアントサンプル
 ソースコードが入手できます。

「週刊ブリュンヒルデ創刊号。」: http://blog.x-row.net/?p=5841

「ブリュンヒルデ零式。」      : http://blog.x-row.net/?p=6818

「ブリュンヒルデ零式改。」    : http://blog.x-row.net/?p=6911

「ブリュンヒルデ零式改弐」    : http://blog.x-row.net/?p=10682

「ブリュンヒルデ零式改参」    : http://blog.x-row.net/?p=11892

 (*) 1番下のURL(↑)からダウンロードできるソースコードが最新です。

 (**) MSVC2015でビルドする場合以下の点に注意する必要がありました。

   1. ソースファイルの改行コードはオリジナルのCR+LFのままとする。

   2. プロジェクトの設定について(Releaseビルド/Debugビルドそれぞれで必要)

      (1) 文字セットとして「Unicode 文字セットを使用する」を選択する。

          マルチバイト文字を選択するとソースファイルの修正が必要となる。

      (2) SDL チェックを無効にする。

   3. (おまけ) ビルド時の警告を抑止するためにはソースファイルの先頭に以下の1行を
      挿入する。

      #define _CRT_SECURE_NO_WARNINGS 1

(3) Qt Projectのページ

 https://www.qt.io/

(4) CELTのページ

 The CELT ultra-low delay audio codec : http://www.celt-codec.org/

(5) OpenSSLのページ

 https://www.openssl.org/

(6) VP8のページ

 https://www.webmproject.org/

----------------------------------------------------------------------
12. 謝辞
----------------------------------------------------------------------

有用なソフトウェアを公開して頂き、また仕様公開して頂いたIchiGekiさんに
感謝します。

また有用なツール、ライブラリを提供くださるQt Projectに感謝します。

最後にGNU, MinGW, Cygwin, CELTライブラリ, OpenSSL, VP8, Inno Setup, 7-Zipなど
フリーなソフトウェアを提供くださるすべての方々に感謝します。

----------------------------------------------------------------------

開発日誌は以下のページにあります。
(内容は開発過程に興味のある方向けです）

http://mcz-xoxo.cocolog-nifty.com/blog/

ソースコードは以下で公開しています。

https://github.com/funfun-dc5/qtbrynhildr.git

Coverity Scanの解析結果は以下で見られます。

https://scan.coverity.com/projects/qt-brynhildr

----------------------------------------------------------------------
[CELT LICENSE]

Copyright 2001-2009 Jean-Marc Valin, Timothy B. Terriberry,
                    CSIRO, and other contributors

Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this li
st of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright notice, this
 list of conditions and the following disclaimer in the documentation and/or oth
er materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AN
D ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOTLIMITED TO, THE IMPLIED W
ARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED
.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN
DIRECT, INCIDENTAL, SPECIAL,EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO,PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, O
R PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILI
TY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHE
RWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF TH
E POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
[OpenSSL LICENSE]

  LICENSE ISSUES
  ==============

  The OpenSSL toolkit stays under a dual license, i.e. both the conditions of
  the OpenSSL License and the original SSLeay license apply to the toolkit.
  See below for the actual license texts. Actually both licenses are BSD-style
  Open Source licenses. In case of any license issues related to OpenSSL
  please contact openssl-core@openssl.org.

  OpenSSL License
  ---------------

/* ====================================================================
 * Copyright (c) 1998-2016 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

 Original SSLeay License
 -----------------------

/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

----------------------------------------------------------------------
[VP8 LICENSE]

Copyright (c) 2010, Google Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

・Redistributions of source code must retain the above copyright notice, this li
st of conditions and the following disclaimer.

・Redistributions in binary form must reproduce the above copyright notice, this
 list of conditions and the following disclaimer in the documentation and/or oth
er materials provided with the distribution.

・Neither the name of Google nor the names of its contributors may be used to en
dorse or promote products derived from this software without specific prior writ
ten permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WA
RRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT
, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, D
ATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF L
IABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE O
R OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE.
