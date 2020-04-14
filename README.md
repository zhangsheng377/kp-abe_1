# kp-abe

1. 我的wsl环境带不带gmp，需要手动安装：
```
   systemctl stop packagekit
   sudo apt install libgmp3-dev
   g++ test_gmp.cpp -o test_gmp -lgmp
   
   PS：自制automake方法：https://blog.csdn.net/u011857683/article/details/82026809
```

2. 虽然没有libclt13.so库文件，但貌似可以直接下载clt源文件的方式去调用，有时间可以试试。
```
   # To add and clone a new submodule
   $ git submodule add https://github.com/5GenCrypto/clt13.git
   $ git clone https://github.com/5GenCrypto/clt13.git
   $ git submodule init
   $ git submodule update
```
```
   sudo apt install cmake
   sudo apt install libflint-dev
   cp zsd_clt_build.sh clt13/build.sh
   dos2unix build.sh
   ./build.sh
   等libaesrand下载完成后,ctrl+c结束脚本运行
   cp ../aesrand.h libaesrand/aesrand/aesrand.h.in
   cp ../clt13.h src/clt13.h
   ./build.sh
   make
   sudo make install
   sudo cp build/include/* /usr/local/include/
   sudo cp build/lib/* /usr/local/lib/
```

3. 接下来都准备好后，就是要调通程序，能跑起来就行
```
   g++ Encryption.cpp util_clt.cpp util_shm.cpp server.cpp -o server -lgmp -lclt13 -laesrand -lexplain
   g++ Encryption.cpp util_clt.cpp util_shm.cpp client.cpp -o client -lgmp -lclt13 -laesrand -lexplain
   LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/
   export LD_LIBRARY_PATH
   ./server
   ./client
```
```
$ ./server
Memory attached at 0x7f1ec4172000
Memory attached at 0x7f1ec4171000
setUp start:
PublicKey init****************************************
  λ: 10
  κ: 3
  α: 10
  β: 10
  η: 91
  ν: 16
  ρ: 10
  ρ_f: 60
  n: 3
  nzs: 3
  ncores: 4
  Flags: 
  Generating g_i's:
10      100% [||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||] 3/3 [0.00s]
  Generating p_i's:
91      100% [||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||] 3/3 [0.00s]
  Computing x0:
        100% [||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||] 2/2 [0.00s]
  Generating CRT coefficients:
        100% [||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||] 3/3 [0.00s]
  Generating z_i's:
        100% [||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||] 3/3 [0.00s]
  Generating pzt:
        100% [||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||] 6/6 [0.01s]
The public gk^a=0 | -1283689163906860756363699258582330293338337858797062930567429193826738936340828211
The public h0=1516104941095744289606004030755462156171649846699029452089230686979579549572456289
The public h1=-1101245791352855750111656139863909011837326963895026345297171970522068778992487831
The public h2=1725497739224590354681727059407140013121850543082530028333023286219175211866936800
The public h3=712221168423874825637472605450484807443555943495415760516460708973264668232816102
The public h4=2007910388324148384879433389890768064519472736140130512435929631712547769854161605
setup time is 62500
setUp over

KeyGen start:
skUnion[0]=623218784081266948797828350484380855113672010005416717994046100901040110680367023
skUnion[1]=625901648966252827465823310950410405299607050148364746872282856212504699514315034
skUnion[2]=3285118035351441936164235755156727569491994895111698116314250133985587056148821673
skUnion[3]=126887463565581471756244135388648432302147788549758628117930932695503775473771940
skUnion[4]=-1751306573145783681435502624273347461883119707327117991505490979319818920333195228
skUnion[5]=3643896706841632317251593794925840596011022088792141596953480348204971650355486732
skUnion[6]=-448667277327225127240803603041825451790592303839269856487210241638739138676715170
KeyGen over

start SendPublicKey
Memory attached at 0x7f1ec4170000
start SendSsk
Memory attached at 0x7f1ec416f000
send over



./client
Send systemParam start:
Memory attached at 0x7f4549b48000
Send systemParam over

Send request start:
Memory attached at 0x7f4549b47000
Send request over

GetPublicKey start:
Memory attached at 0x7f4549b46000
GetPublicKey shm_publicKey->top_level:3
GetPublicKey shm_publicKey->attrNumber:5
GetPublicKey util_clt_pp_restore over
GetPublicKey util_clt_state_restore over
GetPublicKey shm_publicKey->MSK over
GetPublicKey shm_publicKey->attributes over
GetPublicKey shm_publicKey->encodingOfa over
The publicKey encodingOfa=0 | -1283689163906860756363699258582330293338337858797062930567429193826738936340828211
The publicKey attribute[0]=1516104941095744289606004030755462156171649846699029452089230686979579549572456289
The publicKey attribute[1]=-1101245791352855750111656139863909011837326963895026345297171970522068778992487831
The publicKey attribute[2]=1725497739224590354681727059407140013121850543082530028333023286219175211866936800
The publicKey attribute[3]=712221168423874825637472605450484807443555943495415760516460708973264668232816102
The publicKey attribute[4]=2007910388324148384879433389890768064519472736140130512435929631712547769854161605
GetPublicKey over

GetSsk start:
Memory attached at 0x7f4549b45000
GetSsk shm_sk->nodeNumber:3
GetSsk shm_sk->kh over
GetSsk shm_sk->skUnion over
GetSsk shm_sk->skStartIndex over
The sk skUnion[0]=623218784081266948797828350484380855113672010005416717994046100901040110680367023
The sk skUnion[1]=625901648966252827465823310950410405299607050148364746872282856212504699514315034
The sk skUnion[2]=3285118035351441936164235755156727569491994895111698116314250133985587056148821673
The sk skUnion[3]=126887463565581471756244135388648432302147788549758628117930932695503775473771940
The sk skUnion[4]=-1751306573145783681435502624273347461883119707327117991505490979319818920333195228
The sk skUnion[5]=3643896706841632317251593794925840596011022088792141596953480348204971650355486732
The sk skUnion[6]=-448667277327225127240803603041825451790592303839269856487210241638739138676715170
GetSsk over

please input message:269
Encrypt start:
Cm=2153641566880026581924493293866647795209358419492228544091990629567612736147706635
gs=1253312240299989533581270917675339910841575587114002225550693645202662805346751864
c[0]=1168973124359867859288379153481111149142401472545996882852416533870029556330897977
c[1]=3221026712877991276398508646587147120287848284672769617042559503495269370514452078
c[2]=0
c[3]=0
c[4]=0
Encrypt over

Decrypt start:
codeTen=671856726173742775039334441923851483836813464556297066881943460289576183788333414
Decrypt over


The Message is 269
```
```
应该与当前的设置有关，当前只有3个节点，top_level为3，所以3位数的都可还原
```

4. 要实现方案的整体流程：

![流程](picture/frame.png)
