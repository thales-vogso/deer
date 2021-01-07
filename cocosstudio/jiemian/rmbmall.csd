<GameFile>
  <PropertyGroup Name="rmbmall" Type="Node" ID="f13ef886-7789-4e00-b780-759ee6bcb255" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="mall" Tag="20" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="Panel" ActionTag="429633174" Tag="73" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="TopEdge" RightMargin="-960.0000" BottomMargin="-1280.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="960.0000" Y="1280.0000" />
            <Children>
              <AbstractNodeData Name="back2" ActionTag="766779177" Tag="46" IconVisible="False" LeftMargin="-1.0001" RightMargin="1.0001" TopMargin="0.4999" BottomMargin="1069.5001" Scale9Width="960" Scale9Height="210" ctype="ImageViewObjectData">
                <Size X="960.0000" Y="210.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="478.9999" Y="1174.5001" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4990" Y="0.9176" />
                <PreSize X="1.0000" Y="0.1641" />
                <FileData Type="Normal" Path="jiemian/shuijing.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Particle_1" ActionTag="-284552319" Tag="217" IconVisible="True" LeftMargin="480.0000" RightMargin="480.0000" TopMargin="580.0000" BottomMargin="700.0000" ctype="ParticleObjectData">
                <Size X="0.0000" Y="0.0000" />
                <AnchorPoint />
                <Position X="480.0000" Y="700.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5469" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="paticles/deerback.plist" Plist="" />
                <BlendFunc Src="770" Dst="1" />
              </AbstractNodeData>
              <AbstractNodeData Name="backBtnArea" ActionTag="386899819" Tag="112" IconVisible="False" LeftMargin="66.0000" RightMargin="694.0000" TopMargin="22.0000" BottomMargin="1108.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="200.0000" Y="150.0000" />
                <Children>
                  <AbstractNodeData Name="backBtn" ActionTag="-306152040" Tag="23" IconVisible="False" LeftMargin="-16.5000" RightMargin="143.5000" TopMargin="45.5000" BottomMargin="55.5000" FontSize="38" Scale9Width="73" Scale9Height="49" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
                    <Size X="73.0000" Y="49.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="20.0000" Y="80.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1000" Y="0.5333" />
                    <PreSize X="0.3650" Y="0.3267" />
                    <TextColor A="255" R="211" G="213" B="224" />
                    <DisabledFileData Type="Normal" Path="jiemian/jiantou1.png" Plist="" />
                    <PressedFileData Type="Normal" Path="jiemian/jiantou1.png" Plist="" />
                    <NormalFileData Type="Normal" Path="jiemian/jiantou1.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="66.0000" Y="1108.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0688" Y="0.8656" />
                <PreSize X="0.2083" Y="0.1172" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="158730183" Tag="25" IconVisible="False" LeftMargin="400.0000" RightMargin="400.0000" TopMargin="72.4998" BottomMargin="1162.5002" FontSize="40" LabelText="钻石商城" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                <Size X="160.0000" Y="45.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="480.0000" Y="1185.0002" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="226" G="228" B="165" />
                <PrePosition X="0.5000" Y="0.9258" />
                <PreSize X="0.1667" Y="0.0352" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="255" G="127" B="80" />
              </AbstractNodeData>
              <AbstractNodeData Name="scroll" ActionTag="-1227806417" Tag="73" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="TopEdge" TopMargin="180.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="Vertical" ctype="ScrollViewObjectData">
                <Size X="960.0000" Y="1100.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position Y="1100.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.8594" />
                <PreSize X="1.0000" Y="0.8594" />
                <SingleColor A="255" R="255" G="150" B="100" />
                <FirstColor A="255" R="255" G="150" B="100" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
                <InnerNodeSize Width="960" Height="1100" />
              </AbstractNodeData>
              <AbstractNodeData Name="item1" ActionTag="-1873868876" Tag="55" IconVisible="False" LeftMargin="40.0000" RightMargin="520.0000" TopMargin="223.0000" BottomMargin="950.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="194" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="buyBtn" ActionTag="1835686112" Alpha="140" Tag="47" IconVisible="False" LeftMargin="57.6983" RightMargin="56.3017" TopMargin="14.3447" BottomMargin="17.6553" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="50" RightEage="50" Scale9OriginX="50" Scale9Width="110" Scale9Height="75" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
                    <Size X="286.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.6983" Y="55.1553" />
                    <Scale ScaleX="1.4000" ScaleY="1.4000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5017" Y="0.5155" />
                    <PreSize X="0.7150" Y="0.7009" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <PressedFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <NormalFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon" ActionTag="1835231850" Tag="74" IconVisible="False" LeftMargin="20.5000" RightMargin="304.5000" TopMargin="16.5000" BottomMargin="15.5000" Scale9Width="75" Scale9Height="75" ctype="ImageViewObjectData">
                    <Size X="75.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="58.0000" Y="53.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1450" Y="0.4953" />
                    <PreSize X="0.1875" Y="0.7009" />
                    <FileData Type="Normal" Path="jiemian/kuozhanbao.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="name" ActionTag="486187428" Tag="51" IconVisible="False" LeftMargin="163.3468" RightMargin="80.6532" TopMargin="11.9461" BottomMargin="52.0539" FontSize="38" LabelText="50钻石包" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="156.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="73.5539" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="233" G="234" B="208" />
                    <PrePosition X="0.6034" Y="0.6874" />
                    <PreSize X="0.3900" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="price" ActionTag="-1211205711" Tag="52" IconVisible="False" LeftMargin="211.8468" RightMargin="129.1532" TopMargin="54.0629" BottomMargin="9.9371" FontSize="38" LabelText="￥6" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="59.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="31.4371" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="215" G="145" B="71" />
                    <PrePosition X="0.6034" Y="0.2938" />
                    <PreSize X="0.1475" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ownflag" ActionTag="383544076" VisibleForFrame="False" Tag="75" IconVisible="False" LeftMargin="357.0000" RightMargin="-3.0000" TopMargin="62.0000" BottomMargin="-1.0000" Scale9Width="42" Scale9Height="42" ctype="ImageViewObjectData">
                    <Size X="46.0000" Y="46.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="380.0000" Y="22.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9500" Y="0.2056" />
                    <PreSize X="0.1150" Y="0.4299" />
                    <FileData Type="Normal" Path="jiemian/gou.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="240.0000" Y="1057.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2500" Y="0.8258" />
                <PreSize X="0.4167" Y="0.0836" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="item2" ActionTag="193470343" Tag="238" IconVisible="False" LeftMargin="509.5311" RightMargin="50.4689" TopMargin="223.0000" BottomMargin="950.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="194" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="buyBtn" ActionTag="-1966240385" Alpha="140" Tag="239" IconVisible="False" LeftMargin="57.6983" RightMargin="56.3017" TopMargin="14.3447" BottomMargin="17.6553" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="50" RightEage="50" Scale9OriginX="50" Scale9Width="110" Scale9Height="75" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
                    <Size X="286.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.6983" Y="55.1553" />
                    <Scale ScaleX="1.4000" ScaleY="1.4000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5017" Y="0.5155" />
                    <PreSize X="0.7150" Y="0.7009" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <PressedFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <NormalFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon" ActionTag="-373961085" Tag="240" IconVisible="False" LeftMargin="20.5000" RightMargin="304.5000" TopMargin="16.5000" BottomMargin="15.5000" Scale9Width="75" Scale9Height="75" ctype="ImageViewObjectData">
                    <Size X="75.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="58.0000" Y="53.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1450" Y="0.4953" />
                    <PreSize X="0.1875" Y="0.7009" />
                    <FileData Type="Normal" Path="jiemian/kuozhanbao.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="name" ActionTag="-856822469" Tag="241" IconVisible="False" LeftMargin="152.8468" RightMargin="70.1532" TopMargin="11.9461" BottomMargin="52.0539" FontSize="38" LabelText="100钻石包" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="177.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="73.5539" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="233" G="234" B="208" />
                    <PrePosition X="0.6034" Y="0.6874" />
                    <PreSize X="0.4425" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="price" ActionTag="-87310194" Tag="242" IconVisible="False" LeftMargin="201.3468" RightMargin="118.6532" TopMargin="54.0629" BottomMargin="9.9371" FontSize="38" LabelText="￥12" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="80.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="31.4371" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="215" G="145" B="71" />
                    <PrePosition X="0.6034" Y="0.2938" />
                    <PreSize X="0.2000" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ownflag" ActionTag="1119125423" VisibleForFrame="False" Tag="243" IconVisible="False" LeftMargin="357.0000" RightMargin="-3.0000" TopMargin="62.0000" BottomMargin="-1.0000" Scale9Width="42" Scale9Height="42" ctype="ImageViewObjectData">
                    <Size X="46.0000" Y="46.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="380.0000" Y="22.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9500" Y="0.2056" />
                    <PreSize X="0.1150" Y="0.4299" />
                    <FileData Type="Normal" Path="jiemian/gou.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="709.5311" Y="1057.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7391" Y="0.8258" />
                <PreSize X="0.4167" Y="0.0836" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="item3" ActionTag="-2125790956" Tag="74" IconVisible="False" LeftMargin="40.0000" RightMargin="520.0000" TopMargin="389.4238" BottomMargin="783.5762" TouchEnable="True" ClipAble="False" BackColorAlpha="194" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="buyBtn" ActionTag="1833460963" Alpha="140" Tag="75" IconVisible="False" LeftMargin="57.6982" RightMargin="56.3018" TopMargin="14.3448" BottomMargin="17.6552" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="50" RightEage="50" Scale9OriginX="50" Scale9Width="110" Scale9Height="75" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
                    <Size X="286.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.6982" Y="55.1552" />
                    <Scale ScaleX="1.4000" ScaleY="1.4000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5017" Y="0.5155" />
                    <PreSize X="0.7150" Y="0.7009" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <PressedFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <NormalFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon" ActionTag="1814816641" Tag="76" IconVisible="False" LeftMargin="20.5000" RightMargin="304.5000" TopMargin="16.5000" BottomMargin="15.5000" Scale9Width="75" Scale9Height="75" ctype="ImageViewObjectData">
                    <Size X="75.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="58.0000" Y="53.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1450" Y="0.4953" />
                    <PreSize X="0.1875" Y="0.7009" />
                    <FileData Type="Normal" Path="jiemian/kuozhanbao.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="name" ActionTag="868574085" Tag="77" IconVisible="False" LeftMargin="152.8468" RightMargin="70.1532" TopMargin="11.9461" BottomMargin="52.0539" FontSize="38" LabelText="500钻石包" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="177.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="73.5539" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="233" G="234" B="208" />
                    <PrePosition X="0.6034" Y="0.6874" />
                    <PreSize X="0.4425" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="price" ActionTag="-489382520" Tag="78" IconVisible="False" LeftMargin="201.3468" RightMargin="118.6532" TopMargin="54.0629" BottomMargin="9.9371" FontSize="38" LabelText="￥50" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="80.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="31.4371" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="215" G="145" B="71" />
                    <PrePosition X="0.6034" Y="0.2938" />
                    <PreSize X="0.2000" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ownflag" ActionTag="1570050484" VisibleForFrame="False" Tag="79" IconVisible="False" LeftMargin="357.0000" RightMargin="-3.0000" TopMargin="62.0000" BottomMargin="-1.0000" Scale9Width="42" Scale9Height="42" ctype="ImageViewObjectData">
                    <Size X="46.0000" Y="46.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="380.0000" Y="22.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9500" Y="0.2056" />
                    <PreSize X="0.1150" Y="0.4299" />
                    <FileData Type="Normal" Path="jiemian/gou.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="240.0000" Y="890.5762" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2500" Y="0.6958" />
                <PreSize X="0.4167" Y="0.0836" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="item5" ActionTag="878639579" Tag="80" IconVisible="False" LeftMargin="509.5311" RightMargin="50.4689" TopMargin="391.4240" BottomMargin="781.5760" TouchEnable="True" ClipAble="False" BackColorAlpha="194" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="buyBtn" ActionTag="113404476" Alpha="140" Tag="81" IconVisible="False" LeftMargin="57.6983" RightMargin="56.3017" TopMargin="14.3447" BottomMargin="17.6553" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="50" RightEage="50" Scale9OriginX="50" Scale9Width="110" Scale9Height="75" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
                    <Size X="286.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.6983" Y="55.1553" />
                    <Scale ScaleX="1.4000" ScaleY="1.4000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5017" Y="0.5155" />
                    <PreSize X="0.7150" Y="0.7009" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <PressedFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <NormalFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon" ActionTag="558686283" Tag="82" IconVisible="False" LeftMargin="20.5000" RightMargin="304.5000" TopMargin="16.5000" BottomMargin="15.5000" Scale9Width="75" Scale9Height="75" ctype="ImageViewObjectData">
                    <Size X="75.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="58.0000" Y="53.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1450" Y="0.4953" />
                    <PreSize X="0.1875" Y="0.7009" />
                    <FileData Type="Normal" Path="jiemian/kuozhanbao.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="name" ActionTag="-1138791782" Tag="83" IconVisible="False" LeftMargin="142.3468" RightMargin="59.6532" TopMargin="11.9461" BottomMargin="52.0539" FontSize="38" LabelText="1000钻石包" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="198.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="73.5539" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="233" G="234" B="208" />
                    <PrePosition X="0.6034" Y="0.6874" />
                    <PreSize X="0.4950" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="price" ActionTag="-2098319460" Tag="84" IconVisible="False" LeftMargin="201.3468" RightMargin="118.6532" TopMargin="54.0629" BottomMargin="9.9371" FontSize="38" LabelText="￥98" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="80.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="31.4371" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="215" G="145" B="71" />
                    <PrePosition X="0.6034" Y="0.2938" />
                    <PreSize X="0.2000" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ownflag" ActionTag="-772919207" VisibleForFrame="False" Tag="85" IconVisible="False" LeftMargin="357.0000" RightMargin="-3.0000" TopMargin="62.0000" BottomMargin="-1.0000" Scale9Width="42" Scale9Height="42" ctype="ImageViewObjectData">
                    <Size X="46.0000" Y="46.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="380.0000" Y="22.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9500" Y="0.2056" />
                    <PreSize X="0.1150" Y="0.4299" />
                    <FileData Type="Normal" Path="jiemian/gou.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="709.5311" Y="888.5760" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7391" Y="0.6942" />
                <PreSize X="0.4167" Y="0.0836" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="item6" ActionTag="130041535" Tag="86" IconVisible="False" LeftMargin="40.0000" RightMargin="520.0000" TopMargin="550.2531" BottomMargin="622.7469" TouchEnable="True" ClipAble="False" BackColorAlpha="194" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="buyBtn" ActionTag="1190293092" Alpha="140" Tag="87" IconVisible="False" LeftMargin="57.6983" RightMargin="56.3017" TopMargin="14.3447" BottomMargin="17.6553" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="50" RightEage="50" Scale9OriginX="50" Scale9Width="110" Scale9Height="75" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
                    <Size X="286.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.6983" Y="55.1553" />
                    <Scale ScaleX="1.4000" ScaleY="1.4000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5017" Y="0.5155" />
                    <PreSize X="0.7150" Y="0.7009" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <PressedFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <NormalFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon" ActionTag="-1588683219" Tag="88" IconVisible="False" LeftMargin="20.5000" RightMargin="304.5000" TopMargin="16.5000" BottomMargin="15.5000" Scale9Width="75" Scale9Height="75" ctype="ImageViewObjectData">
                    <Size X="75.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="58.0000" Y="53.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1450" Y="0.4953" />
                    <PreSize X="0.1875" Y="0.7009" />
                    <FileData Type="Normal" Path="jiemian/kuozhanbao.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="name" ActionTag="481075283" Tag="89" IconVisible="False" LeftMargin="142.3468" RightMargin="59.6532" TopMargin="11.9461" BottomMargin="52.0539" FontSize="38" LabelText="5000钻石包" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="198.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="73.5539" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="233" G="234" B="208" />
                    <PrePosition X="0.6034" Y="0.6874" />
                    <PreSize X="0.4950" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="price" ActionTag="531139417" Tag="90" IconVisible="False" LeftMargin="190.8468" RightMargin="108.1532" TopMargin="54.0629" BottomMargin="9.9371" FontSize="38" LabelText="￥488" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="101.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="31.4371" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="215" G="145" B="71" />
                    <PrePosition X="0.6034" Y="0.2938" />
                    <PreSize X="0.2525" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ownflag" ActionTag="-944141247" VisibleForFrame="False" Tag="91" IconVisible="False" LeftMargin="357.0000" RightMargin="-3.0000" TopMargin="62.0000" BottomMargin="-1.0000" Scale9Width="42" Scale9Height="42" ctype="ImageViewObjectData">
                    <Size X="46.0000" Y="46.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="380.0000" Y="22.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9500" Y="0.2056" />
                    <PreSize X="0.1150" Y="0.4299" />
                    <FileData Type="Normal" Path="jiemian/gou.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="240.0000" Y="729.7469" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2500" Y="0.5701" />
                <PreSize X="0.4167" Y="0.0836" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="item7" ActionTag="-441157868" Tag="92" IconVisible="False" LeftMargin="509.5313" RightMargin="50.4687" TopMargin="552.6595" BottomMargin="620.3405" TouchEnable="True" ClipAble="False" BackColorAlpha="194" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="buyBtn" ActionTag="-1858502188" Alpha="140" Tag="93" IconVisible="False" LeftMargin="57.6983" RightMargin="56.3017" TopMargin="14.3447" BottomMargin="17.6553" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="50" RightEage="50" Scale9OriginX="50" Scale9Width="110" Scale9Height="75" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
                    <Size X="286.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.6983" Y="55.1553" />
                    <Scale ScaleX="1.4000" ScaleY="1.4000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5017" Y="0.5155" />
                    <PreSize X="0.7150" Y="0.7009" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <PressedFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <NormalFileData Type="Normal" Path="jiemian/ui_button2.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon" ActionTag="1332856484" Tag="94" IconVisible="False" LeftMargin="20.5000" RightMargin="304.5000" TopMargin="16.5000" BottomMargin="15.5000" Scale9Width="75" Scale9Height="75" ctype="ImageViewObjectData">
                    <Size X="75.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="58.0000" Y="53.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1450" Y="0.4953" />
                    <PreSize X="0.1875" Y="0.7009" />
                    <FileData Type="Normal" Path="jiemian/kuozhanbao.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="name" ActionTag="-2066005341" Tag="95" IconVisible="False" LeftMargin="131.8468" RightMargin="49.1532" TopMargin="11.9461" BottomMargin="52.0539" FontSize="38" LabelText="10000钻石包" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="219.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="73.5539" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="233" G="234" B="208" />
                    <PrePosition X="0.6034" Y="0.6874" />
                    <PreSize X="0.5475" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="price" ActionTag="-1628132504" Tag="96" IconVisible="False" LeftMargin="190.8468" RightMargin="108.1532" TopMargin="54.0629" BottomMargin="9.9371" FontSize="38" LabelText="￥648" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
                    <Size X="101.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="241.3468" Y="31.4371" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="215" G="145" B="71" />
                    <PrePosition X="0.6034" Y="0.2938" />
                    <PreSize X="0.2525" Y="0.4019" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="255" G="127" B="80" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ownflag" ActionTag="635870919" VisibleForFrame="False" Tag="97" IconVisible="False" LeftMargin="357.0000" RightMargin="-3.0000" TopMargin="62.0000" BottomMargin="-1.0000" Scale9Width="42" Scale9Height="42" ctype="ImageViewObjectData">
                    <Size X="46.0000" Y="46.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="380.0000" Y="22.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9500" Y="0.2056" />
                    <PreSize X="0.1150" Y="0.4299" />
                    <FileData Type="Normal" Path="jiemian/gou.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="709.5313" Y="727.3405" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7391" Y="0.5682" />
                <PreSize X="0.4167" Y="0.0836" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="rmbTxt" ActionTag="490137651" Tag="213" IconVisible="False" LeftMargin="830.1342" RightMargin="45.8658" TopMargin="69.6846" BottomMargin="1167.3154" FontSize="38" LabelText="3999" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="84.0000" Y="43.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="830.1342" Y="1188.8154" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="242" G="244" B="196" />
                <PrePosition X="0.8647" Y="0.9288" />
                <PreSize X="0.0875" Y="0.0336" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="rmbImage" ActionTag="-1171909861" Tag="212" IconVisible="False" LeftMargin="767.6357" RightMargin="127.3643" TopMargin="62.2290" BottomMargin="1160.7710" Scale9Width="65" Scale9Height="57" ctype="ImageViewObjectData">
                <Size X="65.0000" Y="57.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="800.1357" Y="1189.2710" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8335" Y="0.9291" />
                <PreSize X="0.0677" Y="0.0445" />
                <FileData Type="Normal" Path="manor/image/zuanshiicon.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position Y="-1280.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>