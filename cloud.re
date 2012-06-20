= クラウドと OpenFlow を連携したアプリケーション

//lead{
ネットワーク屋にとって「ネットワークなんてただの土管だろ」という言葉ほ
どカチンと来るものはありません。でもなぜ腹が立つかを考えてみると、これ
が意外と本質を突いた技術論であることに気付きます。つまり、OpenFlow だろ
うが何だろうが、専門外から見ると同じネットワークの土管にしか見えないと
いう動かしがたい現実があるのです。では、どうすればただの土管から進化で
きるでしょうか？
//}

@<img>{dilbert} は私が 10 年来愛用している Dilbert のマウスパッドで、私
の座右の銘が込められています。たくさんの PC やモニタ、周辺機器に囲まれ
た Dilbert いわく「必要なのはアプリだよ!」。

//image[dilbert][高価なハードウェアもアプリが無ければ意味がない][scale=0.1]

これに似たおもしろい例を、シリコンバレーを貫く国道 101 号線沿いにある
「コンピュータ歴史博物館」で発見しました。
@<img>{honeywell_kitchen_computer} は 60 年代に発売された最初の「家庭用
コンピュータ」で、用途はなんと料理のレシピ管理でした。いわば今で言う
Cookpad (@<tt>{http://http://cookpad.com/}) の専用機版です。価格 1 万ド
ルのこの当時としては最先端コンピュータは、ご丁寧にもおまけのエプロンま
でついていましたが、ついに一台も売れませんでした。

//image[honeywell_kitchen_computer][世界初の家庭用コンピュータ (筆者撮影)。使い方が難解すぎて、ついに一台も売れなかった。][scale=0.1]

売れなかった理由は、ハードウェアではなくアプリやユーザインタフェース側
にありました。このコンピュータを使うには 2 週間の専門プログラミングコー
スを受講しなければならず、さらにレシピの出力がなんと 2 進数表記だったの
です。@<tt>{0011101000111001} という画面表示を見て、これがブロッコリー
かニンジンか分かる主婦はいるでしょうか？これは当時の技術者にはきわめて
普通のユーザインタフェースだったわけですが、やはり家庭向けとしては無理
があったようです。もし使いやすいレシピアプリが最初から付属していれば、
こうはならなかったはずです。

つまりせっかくの高性能なハードウェアも、一般向けに使いやすいアプリがな
ければ普及しないということです。しかもハードウェアの常として、もともと
は高価だった機器もいつかは大量生産による低価格化によっていわゆるコモディ
ティ化 (日用品化) の波にさらされます。こうなると、相対的に見てハードウェ
アに対するアプリの重要性はさらに上がります。

これと同じことが OpenFlow にもあてはまります。OpenFlow はデータセンター
のような超巨大環境でのユースケースは増えつつありますが、この規模のプロ
グラミングは一般の開発者には手が届かない上、データセンタ向けアプリは普
通の規模にはオーバースペックで使えません。しかし、かつての
Hadoop/MapReduce の普及がそうだったように、一般の開発者がどんどん
OpenFlow アプリを書くようになれば、普通な環境でのユースケースが増えます。
またそれによって OpenFlow スイッチのコモディティ化も加速するというもの
です。そして、Trema はまさにこうした「普通の」プログラマをターゲットに
設計されています。

この章では、OpenFlow の「キラーアプリ」をいくつか紹介します。これらはデー
タセンターを持っていなくても誰でも動かすことができ、かつ日常にすぐに役
立つ上、OpenFlow の特長をうまく活用した「土管の違いがわかる」アプリです。

== クラウドを部品として使う

最近のアプリケーションは Web アプリケーションとしてクラウド上で提供する
のが流行りです。クラウドにはいくつも良い性質がありますが、ひとつは個人
や並の企業ではとうてい集めきれないほどの膨大なデータが載っているという
ことです。たとえば Twitter のユーザ数は 1.4 億ですし
@<fn>{twitter_user}、facebook は 8 億人分のユーザデータベースと
@<fn>{fb_user}、それぞれの詳細なプロフィールおよび友達関係を蓄えていま
す。もうひとつの良い性質は、こうしたクラウドサービスは外部から利用可能
な API を備えていて、自分のアプリケーションからクラウドサービスを部品の
一つとして使えるということです。

//footnote[twitter_user][2012 年 4 月のデータ]
//footnote[fb_user][2011 年 9 月のデータ]

しかも Ruby はクラウド言語と呼ばれることもあり、豊富なライブラリを使え
ば大抵のクラウドサービスと連携することができます。たとえば、Amazon Web
Services は公式に Ruby のライブラリを公開していますし、Heroku などの
PaaS、Twitter, facebook などの SNS, PayPal などの課金サービスも Ruby か
ら簡単に使うことができます。

ところで OpenFlow はネットワークに API を付けたものと考えることができま
す。この API を通じて、経路制御や ACL、QoS などなどをユーザのニーズに合
わせて実装することができるのでした。つまり、クラウドにもネットワークに
もプログラム可能な API が揃っているのです。これら「クラウド API」と
OpenFlow の「ネットワーク API」を組み合わせて便利な Web サービスが作れ
ないでしょうか？まずはどんなニーズがあるか探してみましょう。

#@warn(OpenFlow を使って経路制御、ACL、QoS の話を書いた章へのリンク)

== ニーズ 1: ルータの設定ができない？

「インターネットの設定ができないんだけど、代わりにやってくれない？」親
や友達からこんな相談をされたことがある人も多いと思います。なぜこんなこ
とが起こるのでしょうか。

企業関係のデータを紹介しましょう。日本の企業の 99% 以上は中小企業ですが、
そんな中小企業の事務所の数は日本に 609.7 万件もあるそうです。今どきどの
会社はどこもインターネット接続はしているでしょうから、ざっくりこれと同
じだけブロードバンドルータがあると言ってしまって良いでしょう。そして、
そんな中小企業のうち 43.7% は自社に IT 部門を持っていません。要は「半分
弱の中小企業は自分でルータの設定ができない」ということです。このため、
そういうのが得意な人に頼むしかないのです。

ではなぜ、みんなルータの設定ができないのでしょうか。これはルータの設定
画面を開けば (ブラウザで @<tt>{http://192.168.0.1/} を開けば出てくるあ
れです) 一目瞭然です。

 * Ethernet 側 IP アドレス設定
 * Ethernet コンフィグレーション設定
 * Proxy DNS 設定
 * DHCP サーバ設定
 * IP マスカレード設定

どうですか。とてもではないですが、素人がこんな設定ができるとは思えませ
ん。さきほどのキッチンコンピュータを思い出す画面です。

こうなってしまう原因の一つは、ブロードバンドルータの設計にあります。ユー
ザインタフェース自体がルータの狭いファームウェア領域に書き込まれている
ため、どうしてもしょぼい UI になってしまうのです。とくに、最近のリッチ
な Web アプリケーションのユーザインタフェースに慣れきった目からすると、
ルータの設定画面はとても見すぼらしく感じます。

== ニーズ 2: 人脈データベース

私の親戚に、田舎で小さな会社を経営している人がいます。その会社のメイン
業務は保険代理店ですが、そのほかにもレンタカーとコインランドリーを兼ね
ていて、事務所には毎日いろんなお客さんがやって来ます。田舎の保険代理店
はとくに人脈が命なので、訪ねてきたお客さんが誰でどこに勤めているかとか、
誰と親戚で誰と知り合いであるかといったことが即座に出てこなければなりま
せん。また、結婚式や葬式などの重要なイベントに必ず出席するといったこと
が欠かせません。いまはこうした業務を個人の記憶力だけでこなしていますが、
こうした情報をうまく効率的にデータベース化する方法はないでしょうか？

== facebook 連携の仕組み

では facebook の Oauth 認証を使って facebook のユーザ ID とフローを結び
付ける方法を説明します。

新規ゲストが Web ブラウザで Google を開こうとすると、http のパケットが
コントローラへ packet_in として上がります。コントローラの packet_in ハ
ンドラはゲストからの http パケットを透過プロキシへ曲げます。透過プロキ
シは http パケットの中身を書き換えることができますので、クラウド上で動
作する Rails のログイン画面へ強制リダイレクトします
(@<img>{transparent_proxy})。

//image[transparent_proxy][ユーザのブラウザ画面をログイン画面へ強制リダイレクト]

ログイン画面では、おなじみの "facebook でログイン" ボタンが表示されます。
このようにユーザにとっては負担の少ない facebook や Twitter アカウントで
のログイン方法を提供することで、サインアップの手間を減らしています。

"facebook でログイン"ボタンを押すと facebook の Oauth 認証がはじまりま
す。まずユーザは facebook のアプリ認証画面に https でリダイレクトされま
す。このとき、コントローラの packet_in ハンドラは、「受け取ったパケット
が https で行き先が facebook であればそのまま出す」という判断をします。
このため、ユーザは facebook のアプリ承認画面に誘導されます
(@<img>{oauth})。

//image[oauth][facebook のアプリ承認画面へのリダイレクト]

アプリ承認が終わると、ユーザはふたたび Oauth で Rails のログイン画面に
リダイレクトされます。このとき、裏では facebook のユーザ情報が Rails の
DB にダウンロードされます。ここで、Rails はこのユーザ情報と MAC アドレ
スを紐付け、ユーザ ID とフローを紐付けます (@<img>{uid_mac})。

//image[uid_mac][facebook のユーザ ID とフローを紐付ける]

コントローラは認証の終わったユーザの MAC アドレスをコントローラへ教え、
コントローラは接続制限の解除とフローの設定をスイッチに対して行います。
これでユーザは認証済となります (@<img>{auth_done}。

//image[auth_done][認証済ユーザの接続制限解除]

これで、認証の済んだユーザは自由にインターネットを使えるようになります。
一方サービス管理者は、facebook のユーザ情報からその人の所属 (会社)、友
人関係、位置情報、性別などを自分のサービスに利用する手筈が整います。

=== 診断スイッチ

これはジョークアプリですが、クラウド + OpenFlow の例として必要なものを
すべて備えています。中身はふつうのラーニングスイッチですが、パケット転
送の判断を診断メーカー (@<tt>{http://shindanmaker.com/}) に問い合わせる
点が違います。

判断の仕組みはこうです。ラーニングスイッチでは転送するときに送信先の
MAC アドレスから FDB を引いて、どのポートに転送するかを決めるのでした。
診断スイッチでは、送信元の MAC アドレスと送信先の MAC アドレスの「相性」
を診断メーカーに問い合わせ、その結果で相性が良ければ転送し、悪ければ破
棄ます。

診断例を実際に見てみましょう (@<img>{shindan_maker})。診断スイッチのク
ラウド側バックエンド、"MAC アドレスの相性調べったー"
(@<tt>{http://shindanmaker.com/189184}) をブラウザで開いてください。こ
れに適当な MAC アドレスをたとえば 「50:1d:92:93:f4:25 と
18:ca:39:ff:fe:0f」という形式で 2 つ入力すると、相性を調べて愉快なメッ
セージを出してくれます。

 * (相性の良い例) 50:1d:92:93:f4:25 と 18:ca:39:ff:fe:0f は美男美女しか
   も相思相愛! コレガ理想のカップルだね!
 * (相性の悪い例) 49:1d:92:93:f4:01 と 18:ca:39:ff:fe:20は何年も絶縁状
   態! そもそも L1 がつながっていません (drop)

//image[shindan_maker][診断メーカーで MAC アドレスの相性を調べる]

この結果を API として問い合わせるライブラリが shindan gem です。いつも
通り "@<tt>{gem install shindan}" で簡単にインストールできます。診断ス
イッチはこの API を使い、パケットがコントローラに上がってくる
packet_in ハンドラで相性を判断、相性が悪ければ (= 診断文字列に drop の
文字が入っていれば) パケットを破棄します。とても簡単ですね。

== OpenStack 

実はクラウドそのものを簡単に作ってしまうことができます。
#@warn(あとで書く)

== まとめ
== 参考文献