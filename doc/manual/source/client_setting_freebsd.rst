.. -*- mode: rst; coding: utf-8-unix -*-

クライアントのインストールと設定 (FreeBSD版)
============================================

1. ダウンロード
----------------

(1).Qt Brynhildrのダウンロード
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

 以下からダウンロードします。

 <https://github.com/funfun-dc5/qtbrynhildr/releases>

   qtbrynhildr-freebsd_x64-v100.tar.xz

2. Qt5パッケージのインストール
-------------------------------

 pkgコマンドを使ってインストールします。 ::

  funfun@freebsd-PC-x64:~$ pkg install qt5

3. Qt5パッケージのアンインストール
-----------------------------------

 pkgコマンドを使ってアンインストールします。 ::

  funfun@freebsd-PC-x64:~$ pkg delete qt5
  funfun@freebsd-PC-x64:~$ pkg autoremove

4. Qt Brynhildrのインストール
------------------------------

 アーカイブファイルを展開します。 ::

  funfun@freebsd-PC-x64:~$ tar xvfJ ./qtbrynhildr-freebsd_x64-v100.tar.xz

5. Qt Brynhildrのアンインストール
----------------------------------

 以下のようにして展開したディレクトリと設定ファイルを含むディレクトリを手動で削除してください。 ::

  funfun@freebsd-PC-x64:~$ rm -rf ./dist_freebsd_x64
  funfun@freebsd-PC-x64:~$ rm -rf ~/.config/mcz-xoxo

6. 起動/終了と設定ファイル
--------------------------

* 設定ファイル

  標準では以下のファイルに保存されます。 ::

   ~/.config/mcz-xoxo/Qt Brynhildr.ini

  オプションにファイル名を与えることで変更できます。
