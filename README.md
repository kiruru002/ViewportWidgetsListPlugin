# UE5_ViewportWidgetsList

## 目次

- [UE5\_ViewportWidgetsList](#ue5_viewportwidgetslist)
	- [目次](#目次)
		- [UE インストールの方法](#ue-インストールの方法)
		- [Visual Studio インストールの方法](#visual-studio-インストールの方法)
		- [プラグインのダウンロード方法](#プラグインのダウンロード方法)
			- [ダウンロードした後のプラグインの配置](#ダウンロードした後のプラグインの配置)
		- [プラグインのビルド](#プラグインのビルド)
		- [プラグインの設定](#プラグインの設定)

このプラグインはダリアさんのZenn投稿記事 ([【UE5】C++を使って今どのWidgetが表示されているのか確認する機構を作る](https://zenn.dev/daria_nicht/articles/ue5-add-viewport-names)) から着想を得て作られています  

### UE インストールの方法

Unreal Engine 5.3 向けに作られたプラグインです  

[Unreal Engine をダウンロードする](https://www.unrealengine.com/ja/download)  
[Unreal Engine をインストールする](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/installing-unreal-engine)  

### Visual Studio インストールの方法

Visual Studio で無くても良いですが、ビルド環境も必要です  
[最新の無料バージョンをダウンロードする](https://visualstudio.microsoft.com/ja/vs/community/)  
このサイトも詳しい→[ue5study.com](https://ue5study.com/how/unrealengine-packaging-visualstudio-settings/)  

### プラグインのダウンロード方法

後悔ページのトップに行き \[Code\] ボタン → \[Download ZIP\] ボタンを押して、ダウンロードします  
[トップ](https://github.com/kiruru002/ViewportWidgetsListPlugin)  
[最新版ダウンロードURL(ダウンロードが行われます)](https://github.com/kiruru002/ViewportWidgetsListPlugin/archive/refs/heads/main.zip)  

#### ダウンロードした後のプラグインの配置

任意の UE プロジェクトを作成し、生成された .uproject ファイルと同じ位置に Plugins という名前のフォルダを作成し、その中の任意の位置に zip ファイルを展開します (展開後の中身をコピーしても可)  

### プラグインのビルド

.uproject を右クリックし、 \[Generate Visual Studio project files.\] を押します。 .sln ファイルが生成されます  
.sln ファイルを開き、プロジェクトを確認したら、ビルドします  

### プラグインの設定

※プラグインの設定についての説明  
