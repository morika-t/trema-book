= 머리말 

저녁을 먹기 전에 집에서 원고를 쓰고 있을 때, 5살 아들이 보육원에서 주워온 돌을 보여주는 것이었습니다. 아무런 특징도 없는 회색 돌 조각이지만 본인에게는 아주 중요한 모양인지 "절대로 버리지 마세요" 라고 부탁까지 하는 것 이었습니다. 시간이 지나자 이런 "보물" 돌 조각이 책상위에 5개 6개 점점 쌓여갔습니다. 

생각해보면 이 책도 그런 돌 조각 모으기 부터 시작되었습니다. 소프트웨어 공학의 대가 제라드 와인버그의 최신 책으로 와인버그의 문장독본 - 자연석공축법 (출판사 翔泳社 刊) 이 있습니다. 이것은 다작가인 와인버그가 직접 쓴 지도서로, 메모나 인용같은 문장의 단편을 "돌" 이라 칭하여 이 돌을 모으거나 재배치함으로 인해 훌륭한 석조 건물이나 아치를 쌓아 올리는 즉 한권의 책을 쓰는 방법을 설명한 명저입니다. 그래, 일단 돌을 모으자. 자연석공축법을 알게된 필자는 즉시 그 일에 착수했습니다. 

이 책을 집필하는데 있어서 모은 돌 조각은 금방 방대한 양이 되었습니다. 
OpenFlow 의 사양서나 논문은 물론 직장에서 동료들과의 잡담, 소스코드의 자투리, 대학에서의 강의, 전자메일의 리스트에서 발견한 투고, 잡지나 웹의 연재 기사 @<fn>{sd}, 기분전환용 독서나 테레비, 옛날에 곧잘 듣곤 했던 라디오 프로그램, 아이폰에 저장되어있던 사진, 여하튼 작고 큰 여러가지 색깔의 돌 조각을 도처에서 마구 모아서 재배치한 나날들이었습니다. 

//footnote[sd][이 책의 몇개의 장들은 SoftwareDesign2011년 12월호〜2012년 5월호에 연재한 이런 야밤에 OpenFlow 로 네트웍을 프로그래밍!［Trema편］』을 대폭 가필, 수정한 것입니다.]

이렇게 모은 돌 중에는 결코 혼자서는 모으지 못했을 귀중한 돌도 많이 있습니다. 이 책의 원고에 GitHub에 공개한 뒤 @<fn>{github}, 많은 분들이 100건 이상의 레뷰를 주셨습니다. 돌 조각을 준 아들 같이, 필자가 부탁하지 않아도 선의로 코멘트를 보내주신 분들이 많이 계십니다. 이런 몇개의 돌들은 이 책의 중요한 부분을 차지하고 있습니다. 특히 <chap>{datacenter_wakame}의 Trema를 사용한 데이터센터, @<chap>{google}의 Google 에서 의 OpenFlow 사용 케이스, 그리고 @<chap>{diy_switch}의 OpenFlow 스위치 자작법은, 쉽게 절대 손에 넣기 어려운 금빛의 보석을 여러 분들이 주신 덕분에, 집필이 가능한 챕터 들이었습니다. 

//footnote[github][@<href>{https://github.com/yasuhito/trema-book}]

"石集め"協力者のみなさん（敬称略、順不同）：壬生 亮太、宮下 一博、石井 秀治、金海 好彦、@<tt>{@stereocat}、高田 将司、富永 光俊、沼野 秀吾、富田 和伸、前川 峻志、園田 健太郎、大山 裕泰、藤原 智弘、空閑 洋平、佛崎 雅弘、阿部 博、笹生 健、山口 啓介、森部 正二朗、高橋 大輔、山本 宏、橋本 匡史、小泉 佑揮、廣田 悠介、長谷川 剛、千葉 靖伸、須堯 一志、下西 英之、角 征典、高橋 征義、早水 悠登、弓削 吉正、村田 友範、上原 正史、高宮 友太郎、高宮 葵、高宮 優子。とくに、以下の方々には特別感謝です：@<tt>{@SRCHACK}、栄 純明、坪井 俊樹、小谷 大祐、黄 恵聖、山崎 泰宏、取口敏憲。

石はただ集めるだけではなく、必要な形に整形したり隙間を埋めたり、さらには磨き上げたりする必要があります。実は、私はもともとネットワークの専門家ではないので、ネットワークに特有な考え方や用語の説明に苦労しました。そうした部分を補ってくれたのが、ネットワーク研究者でありこの本の共著者でもある鈴木一哉氏でした。また私なりにも、石1つひとつの正確さにはベストを尽くしました。たとえばとある章に、酔っぱらいが三軒茶屋（東京都）から武蔵小杉（神奈川県）まで歩いて帰るというエピソードがありますが、私は本当に歩けることを体を張って実証したのです。

執筆と並行してやったのが、本書で取り上げたOpenFlowプログラミングフレームワークTremaの開発です。Tremaはもともと、その場しのぎで書いたソフトウェアを出発として、大量のテストコード、リポジトリサーバーのクラッシュ、@<tt>{svn}から@<tt>{git}への乗り換え、二度の忘年会、いきなりの人事異動、インドとの長距離電話会議、を経験して鍛えられてきたフレームワークです。ずいぶんと曲折を経たものですが、まさしく石のような意思で乗り切りました。

私は、いいフレームワークといい本ができあがったと思っています。私のいいか悪いかの判断基準は、「まだ誰もやっていないことは、いいことだ」という単純な考えに基いています。Tremaみたいなフレームワークは、まだ誰もやってない。OpenFlowを正面きってここまで扱っている本は、まだ他にはない。だからどちらも私にとっては"いいもの"なのです。もちろん、本当にいいか悪いかは、読者のみなさんのご判断におまかせすることにします。

//flushright{
2012年12月1日@<br>{}高宮 安仁（@<tt>{@yasuhito}）
//}