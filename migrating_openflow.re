= 生活ネットワークを OpenFlow に移行する

//lead{
あとは実践あるのみ! 今までの知識を総動員し、自宅や職場のネットワークを OpenFlow 化していろいろ実験してみましょう。
//}

第 I 部、II 部を通じて OpenFlow のプロトコルや動作モデル、そして Trema を使った OpenFlow プログラミングを学んできました。一人前の OpenFlow エンジニアとしてやっていくために必要な基礎知識はすでにひととおり身についたと言えます。

あとはひたすら実践するだけです。今まで手に入れた知識を実際に道具として使い、いま生活しているネットワーク上で OpenFlow を実際に動かしてみるのが一番です。まずは自宅のネットワークを OpenFlow で置き換えましょう。これがうまく行き物足りなくなったら、こんどは職場で小規模に OpenFlow ネットワークを作りましょう。このように徐々に規模を広げて行くのです。

実際にその環境で暮らしてみて、はじめて見えてくるニーズやアイデア、改善案があります。初めて自転車に乗ったときのことを思い出してください。補助輪をはずしただけで最初は派手に転びますが、多少はケガはしつつもあきらめずに練習を繰り返しているうち誰でも乗りこなせるようになります。自転車に乗れれば、隣りの街やそのまた隣りと行動範囲は一気に広がります。しかし補助輪をはずさずにただ考えているだけではどこにも行けません。

== 大ケガしないためのヘルメット

「でも、いきなり OpenFlow に移行してもし大失敗したら……」。そう考えるのが人情です。家のネットワークはともかく、もし職場のネットワークを止めて同僚に迷惑をかけてしまったらどうしよう……。管理者や上司に注意されたらどうしよう……。

本章ではそうした大失敗を防ぐための「ヘルメット」を紹介します。筆者らは、OpenFlow が登場したころから職場ネットワークで実験を始め、それこそ数え切れないほどの失敗を繰り返してきました。とにかく何度も怒られましたが、その経験からうまくやる方法をアドバイスできます。私たちは既存のネットワークを穏便に OpenFlow に移行するテクニックを知っています。ちょっとした OpenFlow コントローラを書くだけで、移行の際に起こりがちなネットワーク障害を簡単に防げるのです。

まずは、私たちの失敗談を振り返らせてください。

== 私たちの失敗談

話は 2009 年 7 月までさかのぼります。OpenFlow が登場したばかりの当時、私たちはさっそくスイッチングハブ相当の OpenFlow コントローラを書いて小さな OpenFlow ネットワークを職場に構築してみました。「おお、ちゃんと動くじゃん!」気を良くした私たちは、こともあろうにこの OpenFlow ネットワークと職場ネットワークとをいきなりつないでしまいました。まあ大丈夫だろうと楽観的に考えていたのです。

しかしすぐにネットワーク障害が起こり、異常に気づいたネットワーク管理者からお叱りのメールを受け取ることになりました。障害時のセットアップを単純化すると@<img>{buggy_controller_setup} のようになります。

//image[buggy_controller_setup][障害を起こしたときのネットワーク構成を単純化したもの][scale=0.5]

//noindent
職場ネットワーク（レガシーネットワークとします）のスイッチにはホストを 2 台つないでおり、スイッチのポート 3 番を OpenFlow スイッチのポート 1 番と接続しました。この OpenFlow スイッチは、私たちが書いたスイッチングハブ相当の OpenFlow コントローラ (仮に BuggyController とします) で制御していました。

=== 障害報告: Host Flapping が起こっている

ネットワーク管理者から届いた障害報告メールには次のようにありました「レガシーネットワークのホストどうしが通信できなくなった。スイッチは Host Flapping 警告を出している。」

Host Flapping とは、1 つのホストがいくつかのポートの間で高速で移動しているように見えるという意味です。「なんでそんなことが起こるんだろう？」われわれはすぐに OpenFlow ネットワークを切断し、そしてもちろんネットワーク管理者にはごめんなさいメールを出してから、原因の分析にとりかかりました。

=== 障害原因が判明

分析の結果、@<img>{failure_analysis} のようなシナリオが起こっているという結論に至りました。

//image[failure_analysis][レガシーネットワークで起こった障害のシナリオ][scale=0.5]

 1. host1 が host2 へパケットを送信する
 2. BuggyController は OpenFlow スイッチポート 1 番からの Packet In を受け取り、OpenFlow スイッチのスイッチポート 1 番に host1 がつながっていると学習する
 3. host2 が host1 へパケットを送信する
 4. BuggyController はスイッチポート 1 番から「宛先＝host1」の Packet In を受け取る。ここで、host1 は OpenFlow スイッチのスイッチポート 1 番にあると学習しているので、スイッチポート 1 番に Packet Out する
 5. 結果的に、host1 はポート 2 と 3 の両方から同じパケットを受け取る。レガシーネットワークのスイッチから見ると、host2 がスイッチポート 2 番と 3 番を高速に移動しているように見える

つまり、BuggyController が予期せぬパケットをレガシーネットワークに送ったおかげでネットワークが大混乱し、通信できない状況が起きたのです。

=== 教訓: これをやってはいけない

振り返ると、失敗した原因は 2 つありました。

 * 1 つは、OpenFlow ネットワークをいきなりレガシーネットワークとつないでしまったことです。OpenFlow ネットワーク単体では動いていたのに、というのは言い訳にはなりません。若気の至りや経験不足から来る「青い」ミスです。
 * もう 1 つは、BuggyController が Packet In と同じポートに Packet Out するという通常あり得ない動作をしていたことです。要所要所で @<tt>{assert} を入れるといった防御的プログラミングや、ユニットテストを徹底していれば@<fn>{unittest}防げるバグでしたが、当時の私たちは動かすことに精いっぱいでそこまで気が回りませんでした。

//footnote[unittest][ユニットテストを使ったコントローラの開発手法については@<chap>{tdd}で解説します。]

//noindent
というわけで、大障害を起こして始めて気付くという最悪のパターンになってしまったわけです。

== OpenFlow への移行パターン

大失敗をやらかしてしまった筆者たちは、OpenFlow 移行のための作戦を練りなおさざるを得なくなりました。いろいろな方向から考えなおしたところ、OpenFlow への移行方法には次の 3 つのパターンがあることがわかりました。もちろん、それぞれでメリット/デメリットや危険度が異なります。

=== A: 独立ネットワークパターン

最初のパターンは、既存のレガシーネットワークにまったく手を加えずに、それとは独立したもうひとつの OpenFlow ネットワークを構築する方法です (@<img>{pattern1})。それぞれのネットワーク間でパケットの行き来はなく、お互いに完全に独立しています。

//image[pattern1][レガシーネットワークとは独立した OpenFlow ネットワークを構築し、徐々に拡大する][scale=0.5]

//noindent
この状態から、レガシーネットワーク内のサーバや端末を徐々に OpenFlow ネットワークに移動することで移行していきます。

それぞれのネットワーク間ではパケットが行き来できないので、OpenFlow ネットワークがレガシーネットワークに悪影響を及ぼす可能性はまずありません。ただし、OpenFlow ネットワークに移行する際には関連する機器どうし（ファイルサーバとクライアント群など）を一度に移行する必要があります。これはトラブルを起こす可能性が高いため、レガシーネットワークの規模が大きい場合には移行が難しいという問題があります。

=== B: いきなり接続パターン

次のパターンは、私たちがやったようにレガシーネットワークと OpenFlow ネットワークをいきなりつなげてしまう方法です（@<img>{pattern2}）。

//image[pattern2][レガシーネットワークと OpenFlow ネットワークを直結してしまう][scale=0.5]

//noindent
相互に通信できるのでネットワーク間でのサーバや端末の移動は自由にできます。このため、独立ネットワークパターンに比べて移行の手間はずっと小さいと言えます。

ただしこの方法は、私たちが失敗したようにとてもリスクの高い方法です。OpenFlow ネットワークのコントローラが完璧に作られていれば、このようにいきなりつなげても問題はありませんが、完璧を期するのはなかなかむずかしいものです。というのも、実際のトラフィックをコントローラに流し込んでみて初めて見つかるバグもあるからです。よって、この方法は自宅ネットワークなど他人に迷惑のかからないネットワーク以外では推奨できません。

=== C: フィルタ経由で接続パターン

最後のパターンは、今までに挙げてきた 2 つのパターンのいいとこどりです。2 つのネットワークを接続するのですが、そのときに「逆流防止フィルタ」となる OpenFlow スイッチを間にはさむことでパケットの逆流が起きないようにします（@<img>{pattern3}）。

//image[pattern3][レガシーネットワークと OpenFlow ネットワークの間での逆流を防止する][scale=0.5]

//noindent
この逆流防止フィルタはたとえば、「レガシーネットワーク → OpenFlow ネットワーク」のような一方向のパケットは通しますが、同じパケットがレガシー側に戻ることを防ぎます。逆方向も同様です。

この方法の利点は、逆流を防ぐだけで今回のケースも含めたかなりの障害を未然に防げることです。また、使い勝手はいきなり接続した場合と同じなので OpenFlow への移行も楽です。ただし、2 つのネットワーク間にもうひとつフィルタ用の OpenFlow スイッチをはさまなければならないという手間はかかります。

== 逆流防止フィルタ

検討の結果、逆流防止フィルタを使ったパターンが一番良さそうでした。フィルタを動かすためのサーバも余っていましたし、何よりコントローラとして簡単に実装できそうだったからです。前置きが長くなりましたが、さっそく Trema で実装してみましょう。

逆流防止フィルタは 1 つの Packet In に対して 2 つのフローを設定します。1 つは順方向のフローで、入ってきたパケットをもう 1 つのスイッチポートに転送します。もう 1 つは逆方向のフローで、同じパケットが逆方向に流れてきたときにこのパケットを落とします。

=== ソースコード

逆流防止フィルタ（OneWayBridge コントローラ）のソースコードを@<list>{oneway_bridge} に示します。このコントローラは、Packet In と Flow Removed のハンドラを定義しています。

//list[oneway_bridge][逆流防止フィルタ (OneWayBridge コントローラ)]{
class OneWayBridge < Controller
  # Packet In で順方向および逆方向のフローを設定する
  def packet_in datapath_id, message
    out_port = { 1 => 2, 2 => 1 }[ message.in_port ]
    add_flow datapath_id, message.macsa, message.in_port, out_port
    send_packet datapath_id, message, out_port
    add_drop_flow datapath_id, message.macsa, out_port
  end


  # 順方向と逆方向のフローのいずれかが消えたときに、もう一方も消す
  def flow_removed datapath_id, message
    delete_flow datapath_id, message.match.dl_src
  end


  # 以下、プライベートメソッド
  private


  # 順方向のフローの設定:
  # 送信元 MAC アドレスが macsa で、スイッチポート in_port から out_port へのフローを追加
  def add_flow datapath_id, macsa, in_port, out_port
    send_flow_mod_add(
      datapath_id,
      :idle_timeout => 10 * 60,
      :match => Match.new( :in_port => in_port, :dl_src => macsa ),
      :actions => SendOutPort.new( out_port )
    )
  end


  # 逆方向のフローの設定:
  # 逆流してきたパケット (送信元 MAC アドレスから判断) を落とす。
  def add_drop_flow datapath_id, macsa, in_port
    send_flow_mod_add(
      datapath_id,
      :idle_timeout => 10 * 60,
      :match => Match.new( :in_port => in_port, :dl_src => macsa )
    )
  end


  # パケットをスイッチポート out_port へ転送
  def send_packet datapath_id, message, out_port
    send_packet_out(
      datapath_id,
      :packet_in => message,
      :actions => SendOutPort.new( out_port )
    )
  end


  # 順方向と逆方向のフローで残っている方を消す
  def delete_flow datapath_id, macsa
    send_flow_mod_delete(
      datapath_id,
      :match => Match.new( :dl_src => macsa )
    )
  end
end
//}

@<tt>{packet_in} ハンドラでは、Packet In したスイッチポートとは別のポートへパケットを転送するフローを設定し（@<tt>{add_flow} メソッド）、Packet In を起こしたパケットを転送します（@<tt>{send_packet} メソッド）。また、同じパケットが逆向きに流れないようにするフローを設定することで逆流を防ぎます（@<tt>{add_drop_flow} メソッド）。同じパケットかどうかは送信元の MAC アドレスが同じかどうかで判断します。

@<tt>{flow_removed} ハンドラは、順方向または逆方向のフローが消えたときに呼ばれます。これらのフローはどちらも @<tt>{:dl_src} に同じ MAC アドレスを指定しているので、@<tt>{delete_flow} メソッドでもう片方の対になるフローを消します。

== 実行してみよう

それではさっそく実行してみましょう。実行のためには、レガシーネットワークと OpenFlow ネットワークの間に OneWayBridge コントローラで制御する仮想スイッチ（vswitch）をはさみます（@<img>{oneway_bridge}）。vswitch のポートは、vswitch を実行するマシンの NIC（eth0、eth1）に結び付けます。

//image[oneway_bridge][逆流防止フィルタ（OneWayBridge コントローラ）を実行するときの物理構成][scale=0.5]

@<img>{oneway_bridge} の物理構成を Trema 設定ファイルにしたものが@<list>{oneway_bridge_conf}（one-way-bridge.conf）です。仮想リンク（link で始まる行）の端点にインターフェース名 eth0、eth1 を指定していることに注目してください。

//list[oneway_bridge_conf][逆流防止フィルタ（OneWayBridgeコントローラ）の設定ファイル]{
vswitch ( "bridge" ) {
  datapath_id 0xabc
}

link "bridge", "eth0"
link "bridge", "eth1"
//}

実行するには、この設定ファイルを trema run の -c オプションに渡します。

//cmd{
% ./trema run ./one-way-bridge.rb -c ./one-way-bridge.conf
//}

=== 職場で使ってみた

さっそくこの逆流防止フィルタを導入したところ、問題は起こらなくなりました。現在、OpenFlow スイッチ 5 台、ホスト約 100 台から構成される OpenFlow ネットワークを職場ネットワークと接続して運用しています。この OpenFlow ネットワークは現在もどんどん拡大しつつあり、その上で OpenFlow を使ったたくさんの研究が生まれています。研究成果の一つとして Trema が生まれたのも、こうした「実践あるのみ」という姿勢があったからに違いありません。

== まとめ

職場のネットワークを安全に OpenFlow に移行するための Tips を学びました。

 * 既存のレガシーネットワークを OpenFlow に移行するいくつかのパターンを見ました。自宅ネットワークなど自由にできるネットワークでは「いきなり接続パターン」で十分ですが、職場ネットワークでは「逆流防止パターン」が最適です。
 * 逆流防止フィルタを実現する OpenFlow コントローラを実装しました。基本的には 2 つのフローを設定するだけで、簡単に逆流を防止できます。
