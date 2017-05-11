.. -*- mode: rst; coding: utf-8-unix -*-

クライアントのインストールと設定 (Windows版)
============================================

1. ダウンロード
---------------

(1) Qt Brynhildrのダウンロード
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 以下からダウンロードします。

 <https://github.com/funfun-dc5/qtbrynhildr/releases>

(2) Visual C++ 2015 Redistributable のダウンロード
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 Windows版ではランタイムライブラリパッケージ

  Visual C++ 2015 x86 Redistributable または Visual C++ 2015 x64 Redistributable

 が必要な場合があります。例えば以下から入手できます。

 <https://www.microsoft.com/ja-JP/download/details.aspx?id=48145>

2. インストール
---------------

インストーラパッケージを実行します。インストールウィザードに従ってインストールします。

(1) 言語の選択
^^^^^^^^^^^^^^

「日本語」もしくは「英語」を選択します。

.. image:: images/windows_installer1.jpg

(2) セットアップウィザードの開始
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. image:: images/windows_installer2.jpg

(3) 使用許諾の同意
^^^^^^^^^^^^^^^^^^
.. image:: images/windows_installer3.jpg

「 同意する」を選択します。

.. image:: images/windows_installer4.jpg

(4) インストール先の指定
^^^^^^^^^^^^^^^^^^^^^^^^
 インストール先を指定します。

.. image:: images/windows_installer5.jpg

(5) プログラムグループの指定
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 登録するスタートアップメニューのグループを指定します。

.. image:: images/windows_installer6.jpg

(6) 追加タスクの選択
^^^^^^^^^^^^^^^^^^^^

 必要な作業にチェックを入れてください。

.. image:: images/windows_installer7.jpg

(7) インストール準備完了
^^^^^^^^^^^^^^^^^^^^^^^^
.. image:: images/windows_installer8.jpg

(8) セットアップウィザードの完了
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. image:: images/windows_installer9.jpg

3. アンインストール
-------------------

(1) 「プログラムと機能」を開く

.. image:: images/windows_uninstaller2.png


(2) 「アンインストール」を実行する

確認ダイアログが表示されます。

.. image:: images/windows_uninstaller3.jpg

「アンインストール」が実行されます。

.. image:: images/windows_uninstaller4.jpg

「アンインストール」が完了しました。

.. image:: images/windows_uninstaller5.jpg


4. 起動/終了と設定ファイル
--------------------------

* 設定ファイル

  標準では以下のファイルに保存されます。::

   C:\Users\ユーザ名\AppData\Roaming\mcz-xoxo\Qt Brynhildr.ini

  オプションにファイル名を与えることで変更できます。
