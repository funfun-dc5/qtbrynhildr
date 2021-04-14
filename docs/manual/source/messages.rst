.. -*- mode: rst; coding: utf-8-unix -*-

メッセージとその対処
=======================

1. 「サーバのサウンドデバイスをチェックしてください！(Please check server sound device!)」

 サーバのサウンド設定の変更が必要かもしれません。
 サーバのサウンドの再生デバイスプロパティのスピーカー周波数を

 「16ビット、44100Hz(CDの音質)」

 に変更すれば改善されるかもしれません。

2. 「サウンドデバイスが見つかりません!(Not found sound device!)」

 サウンドを再生するためのデバイスが見つかりません。
 サウンド再生が正しくできることを確認してください。サウンド再生は強制的にOFFとなります。

3. 「サーバとの接続に失敗しました！(Failed to connect server!)」

 サーバに接続できませんでした。
 サーバ名もしくはIPアドレスが間違えているかもしれません。

4. 「パスワードが違います！(Password Error!)」

 パスワードが間違えているかもしれません。

5. 「すでに他のクライアントが接続しています！(Already connected another client!)」

 すでにサーバに他のクライアントが接続しているかもしれません。

6. 「パブリックモードでサポートしていないビデオコーデックです!(Not support Video Codec!)」

 パブリックモードでサポートしていないビデオコーデックがサーバで指定されています。 [#]_
 brynhildr.iniでvideocodec=2(MotionJPEG)もしくはvideocodec=3(Compress)と指定していること、もしくはvideocodec自体の指定がないことを確認してください。

 .. [#] 現在のところvideocodec=0(Speed)/1(Hybrid)はサポートしていません。

7. 「サーバとの接続にタイムアウトしました！(Timeout to connect server!)」

 "Qt Brynhildr.ini"のtimeoutTimeで設定されている時間だけ待ちましたがサーバに接続できませんでした。
 サーバ名もしくはIPアドレスが間違えているかもしれません。
