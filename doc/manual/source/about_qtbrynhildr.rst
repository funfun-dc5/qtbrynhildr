.. -*- mode: rst; coding: utf-8-unix -*-

Qt Brynhildrとは
===========================

 このソフトウェアは **仕様の公開されているパブリックモード** で動作させた
 Brynhildrサーバに接続するためのクライアントソフトウェア [#]_ です。

 サーバ機能は今の所持っていませんので、サーバとしては
 Brynhildr  [#]_ (最新を推奨)をご利用ください。

 **また、公開されているパブリックモードでは現状ファイルの送受信、クリップボードの
 共有はできない仕様になっていますので、これらの機能が必要な場合はBrynhildrの
 クライアントモードをご利用ください。**

 Brynhildrの標準のクライアントモードに対して大きなアドバンテージのある
 クライアントではありませんのでご承知おきください。
 このソフトウェアはより多くの環境(OS)でBrynhildrのリモートコントロール機能を
 利用できるようにすることを主たる目的としています。
 (あと作者のC++/Qt/Windows/Linuxでのソフトウェア開発の個人的な勉強のため…)

 また、パブリックモードでは通常のモードに比べてセキュリティレベルが低下します。
 重要な操作を行うような場合は通信路の暗号化などの対策を検討したほうがよいかも
 しれません。


 .. [#] 簡単に言うと「目の前にないWindowsのPCを他のPCなどから操作できる」ようにするための
        ソフトウェアです。操作したいPCに別途サーバソフトウェアをインストールする必要があります。

 .. [#] パブリックモードを有効にしたSiegfriedにも接続できます。SiegfriedはBrynhildrの原作者
        IchiGekiさんが公開されているリモートデスクトップソフトウェアです。


 Windows版 Qt Brynhildr

 .. image:: images/qtbrynhildr_on_windows.jpg

           
Linux(Ubuntu)版 Qt Brynhildr

 .. image:: images/qtbrynhildr_on_ubuntu.png

 Mac OSX版 Qt Brynhildr

 .. image:: images/qtbrynhildr_on_macosx.png
