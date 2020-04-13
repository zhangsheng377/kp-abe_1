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
Memory attached at 0x7fd1a7a6b000
buildTree end
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
        100% [||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||] 6/6 [0.00s]
The public gk^a=0 | 1354004704232040351385179187467239593556624724763172105545864354530433993053073915
The public h0=-1342498031984944781025993152571717656685280670731538085002982480987641536123289510
The public h1=340419320893030206388495849518658970397084973636543068638205956471785469605210889
The public h2=-1705032674836207869480316908996516179661375427445488256780957727972430807542369052
The public h3=-155646118680628962947720943112406807346685354489322699423069292707512155793246888
The public h4=-1122075474368880211375334340668242868403404536138622218618988777647824819887314052
setup time is 62500
*********setUp() Complete!!!***************************************************
skUnion[0]=-847314617034918848061260645700488109404669040370243014267690163404934676323244330
skUnion[1]=302770892526234546420972906941436061706981739728209220933050520623884392760422571
skUnion[2]=1999877345089135845243083730491882856453639858260259359875822009369771048697296639
skUnion[3]=1699896998378207325641982028005714891341587123591608006422831751828506176231438633
skUnion[4]=-484189674860666895039595018875725376792152198997618229411234442538767197150500476
skUnion[5]=2255808225817440394098456243397754608748428539300917611971085296882056156966713434
skUnion[6]=-486563481600404939072322093015636672532230376269479953639043867687666526353965522
*********KeyGen() Complete********************************************
start SendPublicKey
Memory attached at 0x7fd1a7a6a000
start SendSsk
Memory attached at 0x7fd1a7a69000
send over


./client
Memory attached at 0x7f900c6fb000
buildTree end
Memory attached at 0x7f900c6fa000
GetPublicKey shm_publicKey->top_level:3
GetPublicKey shm_publicKey->attrNumber:5
GetPublicKey util_clt_pp_restore over
GetPublicKey util_clt_state_restore over
GetPublicKey shm_publicKey->MSK over
GetPublicKey shm_publicKey->attributes over
GetPublicKey shm_publicKey->encodingOfa over
The public gk^a=0 | 1354004704232040351385179187467239593556624724763172105545864354530433993053073915
The public h0=-1342498031984944781025993152571717656685280670731538085002982480987641536123289510
The public h1=340419320893030206388495849518658970397084973636543068638205956471785469605210889
The public h2=-1705032674836207869480316908996516179661375427445488256780957727972430807542369052
The public h3=-155646118680628962947720943112406807346685354489322699423069292707512155793246888
The public h4=-1122075474368880211375334340668242868403404536138622218618988777647824819887314052
*********GetPublicKey() Complete********************************************
Memory attached at 0x7f900c6f9000
GetSsk shm_sk->nodeNumber:3
GetSsk shm_sk->kh over
GetSsk shm_sk->skUnion over
GetSsk shm_sk->skStartIndex over
skUnion[0]=-847314617034918848061260645700488109404669040370243014267690163404934676323244330
skUnion[1]=302770892526234546420972906941436061706981739728209220933050520623884392760422571
skUnion[2]=1999877345089135845243083730491882856453639858260259359875822009369771048697296639
skUnion[3]=1699896998378207325641982028005714891341587123591608006422831751828506176231438633
skUnion[4]=-484189674860666895039595018875725376792152198997618229411234442538767197150500476
skUnion[5]=2255808225817440394098456243397754608748428539300917611971085296882056156966713434
skUnion[6]=-486563481600404939072322093015636672532230376269479953639043867687666526353965522
*********GetSsk() Complete********************************************
please input message:469
Cm=3421450549156400064277657568708406612113360315458821156024787873505336108023082102
gs=1038357499132375870583379208602713972791262508186700868084777156395714229518457136
c[0]=1241310439177891246206009010299532976707672756185315021398582174516319709124202627
c[1]=956157469944657857266515886519155661653016792103674938542849294544353084781830684
c[2]=0
c[3]=0
c[4]=0
*********Encrypt() Complete********************************************
CT*=0 | 976479339377402794875484155070682929209749533317026408897767283169452342637429256
*********Transform() Complete********************************************
codeTen=1404536219549567979826501161659834497634066406993480211131895310801486144942518872
*********Decrypt() Complete********************************************

The Message is 469
```
```
应该与当前的设置有关，当前只有3个节点，top_level为3，所以3位数的都可还原
```

4. 要实现方案的整体流程：

![流程](picture/frame.png)
