/****************************************************************************
** Form implementation generated from reading ui file 'loginform1.ui'
**
** Created: Fri Oct 21 16:48:58 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "loginform1.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "loginform1.ui.h"
static const char* const image0_data[] = { 
"300 3 206 2",
".# c #2f53af",
"bl c #3053b0",
".a c #3054b0",
"bk c #3154b0",
".b c #3155b0",
".c c #3255b1",
"bj c #3356b1",
".d c #3357b1",
".e c #3458b2",
"bi c #3558b2",
".f c #3659b2",
".g c #375ab2",
".h c #385bb3",
".i c #395cb3",
".j c #3a5db4",
"bh c #3b5db4",
".k c #3b5eb4",
".l c #3c5fb5",
"bg c #3d60b5",
".m c #3e60b5",
"bf c #3e61b5",
".n c #3f61b6",
".o c #3f62b6",
"be c #4062b6",
".p c #4163b7",
".q c #4264b7",
".r c #4365b7",
".s c #4466b8",
"Qt c #455a9a",
".t c #4567b8",
"bd c #4668b9",
".u c #4768b9",
".v c #4769b9",
"bc c #486aba",
".w c #496aba",
".x c #4a6bba",
".y c #4b6cbb",
".z c #4c6dbb",
".A c #4d6ebc",
"bb c #4e6fbc",
".B c #4f6fbc",
".C c #4f70bc",
"ba c #5071bd",
".D c #5171bd",
".E c #5272bd",
".F c #5373be",
".G c #5474be",
".H c #5575bf",
".I c #5676bf",
".J c #5777c0",
"b# c #5878c0",
".K c #5978c0",
"b. c #5a79c0",
".L c #5a79c1",
".M c #5b7ac1",
"a9 c #5c7bc1",
".N c #5c7bc2",
".O c #5d7cc2",
".P c #5e7dc2",
".Q c #5f7ec3",
".R c #607fc3",
"a8 c #6180c4",
".S c #6280c4",
".T c #6381c4",
".U c #6482c5",
".V c #6583c5",
".W c #6684c5",
".X c #6785c6",
".Y c #6886c6",
"a7 c #6987c7",
".Z c #6a87c7",
".0 c #6b88c7",
".1 c #6c89c8",
".2 c #6d8ac8",
".3 c #6e8bc9",
".4 c #6f8cc9",
".5 c #708dca",
"a6 c #718eca",
".6 c #728fca",
".7 c #738fcb",
"a5 c #7490cb",
".8 c #7491cb",
"a4 c #7591cb",
".9 c #7592cb",
"#. c #7692cc",
"a3 c #7793cc",
"## c #7794cc",
"#a c #7894cd",
"a2 c #7995cd",
"#b c #7a96cd",
"a1 c #7b96ce",
"#c c #7b97ce",
"#d c #7c97ce",
"a0 c #7d98ce",
"#e c #7d99cf",
"#f c #7e99cf",
"aZ c #7f9acf",
"#g c #7f9bd0",
"#h c #809cd0",
"aY c #819cd0",
"#i c #829dd0",
"#j c #839ed1",
"#k c #849fd1",
"aX c #859fd2",
"#l c #85a0d2",
"#m c #86a1d2",
"aW c #87a1d3",
"#n c #87a2d3",
"#o c #88a3d3",
"#p c #89a4d4",
"aV c #8aa4d4",
"#q c #8ba5d4",
"#r c #8ca6d4",
"#s c #8da7d5",
"#t c #8ea8d5",
"#u c #8fa9d6",
"#v c #90aad6",
"#w c #91abd7",
"aU c #92acd7",
"#x c #93acd7",
"#y c #94add8",
"aT c #94aed8",
"#z c #95aed8",
"aS c #96afd8",
"#A c #96afd9",
"#B c #97b0d9",
"aR c #98b1d9",
"#C c #98b1da",
"#D c #99b2da",
"aQ c #9ab3da",
"#E c #9bb3db",
"#F c #9cb4db",
"aP c #9cb5db",
"#G c #9db5db",
"#H c #9eb6dc",
"#I c #9fb7dc",
"#J c #a0b8dd",
"#K c #a1b9dd",
"aO c #a2bade",
"#L c #a3bade",
"aN c #a3bbde",
"#M c #a4bbde",
"#N c #a4bcde",
"aM c #a5bddf",
"#O c #a6bddf",
"#P c #a7bedf",
"#Q c #a8bfe0",
"#R c #a9c0e0",
"#S c #aac1e1",
"#T c #abc2e1",
"#U c #acc3e2",
"aL c #adc4e2",
"#V c #aec4e2",
"#W c #afc5e3",
"#X c #b0c6e3",
"aK c #b1c7e3",
"#Y c #b1c7e4",
"#Z c #b2c8e4",
"#0 c #b3c9e4",
"#1 c #b4cae5",
"aJ c #b5cbe5",
"#2 c #b6cbe5",
"#3 c #b7cce6",
"#4 c #b8cde6",
"#5 c #b9cee7",
"#6 c #bacfe7",
"aI c #bbd0e7",
"#7 c #bbd0e8",
"#8 c #bcd1e8",
"aH c #bdd2e8",
"#9 c #bed2e8",
"a. c #bfd3e9",
"a# c #c0d4e9",
"aG c #c1d5ea",
"aa c #c1d6ea",
"ab c #c2d6ea",
"aF c #c3d7eb",
"ac c #c3d8eb",
"ad c #c4d8eb",
"aE c #c5d9ec",
"ae c #c6daec",
"aD c #c7daec",
"af c #c7dbec",
"ag c #c8dbed",
"aC c #c9dced",
"ah c #c9dded",
"ai c #cadeed",
"aB c #cbdeee",
"aj c #cbdfee",
"aA c #ccdfee",
"ak c #cce0ee",
"al c #cde0ef",
"az c #cee1ef",
"am c #cfe2ef",
"an c #d0e3f0",
"ay c #d1e3f0",
"ao c #d1e4f0",
"ap c #d2e5f1",
"ax c #d3e5f1",
"aq c #d3e6f1",
"aw c #d4e7f1",
"ar c #d4e7f2",
"as c #d5e8f2",
"av c #d6e8f2",
"at c #d7e9f3",
"au c #d8eaf3",
"Qt.#.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.v.w.x.y.z.A.B.C.D.E.F.G.H.I.J.K.L.M.N.O.P.Q.R.S.T.U.V.W.X.Y.Z.0.1.2.3.4.5.6.7.8.9#.###a#b#c#d#e#f#g#h#i#j#k#l#m#n#o#p#q#r#s#t#u#v#w#x#y#z#A#B#C#D#E#F#G#H#I#J#K#L#M#N#O#P#Q#R#S#T#U#V#W#X#Y#Z#0#1#2#3#4#5#6#7#8#9a.a#aaabacadaeafagahaiajakalamanaoapaqarasatauavasawaxapayanazalaAaBaiaCagaDaEadaFabaGa#a.aH#8aI#6#5#4#3aJ#1#0#ZaK#X#WaL#U#T#S#R#Q#PaM#NaNaO#K#J#I#HaP#FaQ#DaR#BaSaT#yaU#w#v#u#t#s#raV#p#oaW#maX#k#jaY#haZ#fa0#da1a2#aa3#.a4a5.7a6.5.4.3.2.1.0a7.Y.X.W.V.U.Ta8"
".R.Q.P.Oa9.Mb.b#.J.I.H.G.F.Eba.Cbb.A.z.y.xbc.vbd.t.s.r.qbe.obfbg.lbh.j.h.h.gbi.ebj.cbkbl.#",
"Qt.#.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.v.w.x.y.z.A.B.C.D.E.F.G.H.I.J.K.L.M.N.O.P.Q.R.S.T.U.V.W.X.Y.Z.0.1.2.3.4.5.6.7.8.9#.###a#b#c#d#e#f#g#h#i#j#k#l#m#n#o#p#q#r#s#t#u#v#w#x#y#z#A#B#C#D#E#F#G#H#I#J#K#L#M#N#O#P#Q#R#S#T#U#V#W#X#Y#Z#0#1#2#3#4#5#6#7#8#9a.a#aaabacadaeafagahaiajakalamanaoapaqarasatauavasawaxapayanazalaAaBaiaCagaDaEadaFabaGa#a.aH#8aI#6#5#4#3aJ#1#0#ZaK#X#WaL#U#T#S#R#Q#PaM#NaNaO#K#J#I#HaP#FaQ#DaR#BaSaT#yaU#w#v#u#t#s#raV#p#oaW#maX#k#jaY#haZ#fa0#da1a2#aa3#.a4a5.7a6.5.4.3.2.1.0a7.Y.X.W.V.U.Ta8"
".R.Q.P.Oa9.Mb.b#.J.I.H.G.F.Eba.Cbb.A.z.y.xbc.vbd.t.s.r.qbe.obfbg.lbh.j.h.h.gbi.ebj.cbkbl.#",
"Qt.#.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.v.w.x.y.z.A.B.C.D.E.F.G.H.I.J.K.L.M.N.O.P.Q.R.S.T.U.V.W.X.Y.Z.0.1.2.3.4.5.6.7.8.9#.###a#b#c#d#e#f#g#h#i#j#k#l#m#n#o#p#q#r#s#t#u#v#w#x#y#z#A#B#C#D#E#F#G#H#I#J#K#L#M#N#O#P#Q#R#S#T#U#V#W#X#Y#Z#0#1#2#3#4#5#6#7#8#9a.a#aaabacadaeafagahaiajakalamanaoapaqarasatauavasawaxapayanazalaAaBaiaCagaDaEadaFabaGa#a.aH#8aI#6#5#4#3aJ#1#0#ZaK#X#WaL#U#T#S#R#Q#PaM#NaNaO#K#J#I#HaP#FaQ#DaR#BaSaT#yaU#w#v#u#t#s#raV#p#oaW#maX#k#jaY#haZ#fa0#da1a2#aa3#.a4a5.7a6.5.4.3.2.1.0a7.Y.X.W.V.U.Ta8"
".R.Q.P.Oa9.Mb.b#.J.I.H.G.F.Eba.Cbb.A.z.y.xbc.vbd.t.s.r.qbe.obfbg.lbh.j.h.h.gbi.ebj.cbkbl.#"};

static const char* const image1_data[] = { 
"400 60 432 2",
"#V c #000000",
"#S c #010101",
"#T c #020202",
"#R c #020203",
"cq c #020303",
"#U c #030303",
"bV c #030304",
"bl c #030404",
"a9 c #040403",
"#D c #040404",
"bU c #040405",
"ar c #040505",
"cj c #050504",
"bg c #050505",
"a3 c #050506",
"bt c #050606",
"bT c #050607",
"bN c #060505",
"b7 c #060606",
"cM c #060607",
"aJ c #060707",
"aQ c #060708",
"eD c #070606",
"db c #070707",
"bz c #070708",
"dt c #070808",
"bq c #070809",
"bk c #07090a",
"aq c #07090b",
"ee c #080708",
"dh c #080808",
"em c #080809",
"da c #080909",
"aI c #08090a",
"dv c #08090b",
"#C c #080a0a",
"#E c #080a0b",
"#8 c #090909",
"dc c #090a0a",
"di c #090a0b",
"eC c #090a0c",
"en c #090b0b",
"cC c #090b0c",
"aP c #090b0d",
"eA c #090c0d",
"#k c #0a0a0a",
"aC c #0a0a0b",
"b# c #0a0b0b",
"#l c #0a0b0c",
"eM c #0a0b0d",
"eI c #0a0c0c",
"cH c #0a0c0d",
"bf c #0a0c0e",
"du c #0a0d0e",
"co c #0a0d0f",
"bC c #0b0b0b",
"cO c #0b0c0c",
"cT c #0b0c0d",
"d7 c #0b0c0e",
"d# c #0b0d0e",
"cu c #0b0d0f",
"bS c #0b0e0f",
"bX c #0b0e10",
"bW c #0c0c0c",
"ef c #0c0e0f",
"a8 c #0c0e10",
"bL c #0c0f10",
"ap c #0c0f11",
"ck c #0c0f12",
"#Q c #0c1012",
"#9 c #0c1013",
"d9 c #0c1114",
"dp c #0c1216",
"dl c #0d0d0c",
"#j c #0d0d0d",
"bA c #0d1012",
"ci c #0d1013",
"bM c #0d1113",
"d0 c #0d1114",
"bw c #0d1215",
"dx c #0d1216",
"cJ c #0d1317",
"dw c #0d1418",
"cy c #0d1419",
"#i c #0e0e0e",
"dL c #0e1114",
"c5 c #0e1214",
"aD c #0e1216",
"d. c #0e1316",
"eL c #0e1317",
"e. c #0e1419",
"dE c #0e151a",
"ed c #0e161a",
"bO c #0e161b",
"dF c #0e171c",
"eq c #0e171d",
"#h c #0f0f0f",
"dD c #0f1215",
"dH c #0f1315",
"aO c #0f1316",
"ah c #0f1417",
"ba c #0f151a",
"aW c #0f161b",
"dy c #0f171c",
"dY c #0f181d",
"cP c #0f181e",
"cr c #0f191f",
"eB c #0f1a21",
"bG c #10100f",
"#g c #101010",
"b2 c #101417",
"bE c #101418",
"bs c #101518",
"dN c #101519",
"ev c #10161a",
"eu c #10161b",
"dM c #10171c",
"dA c #10181e",
"dT c #10191f",
"b8 c #101a20",
"d2 c #101a21",
"ej c #101b23",
"cD c #101c24",
"#f c #111111",
"dS c #111518",
"ao c #111519",
"bj c #111619",
"c# c #11161a",
"d5 c #11171b",
"et c #11171c",
"#m c #11191e",
"dG c #111a21",
"eQ c #111c23",
"bH c #111c24",
"dP c #111d25",
"d8 c #111f28",
"c7 c #11202a",
"#e c #121212",
"dO c #12171a",
"cG c #12171b",
"bh c #12181b",
"cL c #12181c",
"dd c #12181d",
"c9 c #12191e",
"ew c #12191f",
"eg c #121a1e",
"dr c #121b21",
"eJ c #121d25",
"eN c #121d26",
"e# c #121e25",
"d3 c #121f28",
"cv c #12202a",
"bY c #12212b",
"df c #12222c",
"cK c #12232d",
"dm c #12232e",
"#b c #131313",
"#c c #131314",
"#d c #131413",
"aB c #131414",
"cB c #13181b",
"b3 c #13181c",
"cn c #13191d",
"ag c #13191e",
"cZ c #131c22",
"el c #131e25",
"bD c #13212b",
"dz c #13222b",
"cl c #13222c",
"ek c #13232d",
"cE c #13232e",
"cs c #13242e",
"cz c #13242f",
"aV c #141313",
"aU c #141314",
"#7 c #141413",
"#a c #141414",
"#P c #141a1e",
"a2 c #141b1f",
"cY c #141c22",
"cX c #141d23",
"dg c #141e25",
"es c #141e26",
"a. c #142028",
"dJ c #14212a",
"d4 c #14222b",
"bx c #14222d",
"eH c #14232d",
"cm c #14242e",
"bP c #14242f",
"bI c #142430",
"## c #151515",
"ca c #151b1f",
"a7 c #151b20",
"eb c #151c20",
"eT c #151c21",
"ex c #151d24",
"c0 c #151e23",
"eo c #151e24",
"cW c #151f25",
"dn c #152129",
"aE c #15222b",
"dU c #15232d",
"eS c #15242d",
"bb c #15242e",
"bm c #15242f",
"dQ c #152430",
"bZ c #15252f",
"b9 c #152530",
"#. c #161616",
"ct c #161c20",
"ch c #161c21",
"dR c #161d23",
"ea c #161e23",
"c1 c #162127",
"c8 c #162129",
"er c #16232c",
"aX c #16252f",
"aY c #162530",
"dq c #16262f",
"aZ c #162630",
".9 c #171717",
"an c #171d21",
"aN c #171d22",
"ec c #171e22",
"aK c #171e23",
"eR c #171f24",
"cV c #17222a",
"#n c #17252e",
"b0 c #17252f",
"bJ c #17262f",
"aa c #172630",
".8 c #181818",
"bB c #181e23",
"cc c #181f23",
"#O c #181f24",
"eG c #181f25",
"c4 c #182025",
"dV c #182127",
"de c #182229",
"#o c #182630",
"bK c #182730",
"a# c #182731",
".7 c #191919",
"d1 c #192024",
"cI c #192025",
"dk c #192127",
"do c #19232a",
"c2 c #19252c",
"cU c #19252e",
"#p c #192730",
"bc c #192731",
".4 c #1a1a1a",
".6 c #1a1a1b",
".3 c #1a1b1a",
"aA c #1a1b1b",
"b4 c #1a2126",
"br c #1a2127",
"c6 c #1a2227",
"cA c #1a2228",
"eK c #1a2328",
"#B c #1a2329",
"ey c #1a262f",
"ab c #1a2730",
"#q c #1a2731",
"#r c #1a2831",
".5 c #1b1a1a",
"#6 c #1b1a1b",
"az c #1b1b1a",
".2 c #1b1b1b",
"cp c #1b2227",
"b6 c #1b2328",
"cb c #1b2329",
"eO c #1b242a",
"dj c #1b252d",
"ds c #1b262d",
"ep c #1b262e",
"eh c #1b272e",
"eE c #1b2731",
"c3 c #1b2830",
"#s c #1b2831",
"bn c #1b2832",
"#v c #1b2931",
"bd c #1b2932",
".1 c #1c1c1c",
"#N c #1c2429",
"b5 c #1c242a",
"eP c #1c262c",
"d6 c #1c2830",
"#t c #1c2831",
"aF c #1c2832",
"#w c #1c2931",
"#u c #1c2932",
"ez c #1c2933",
"bo c #1c2a31",
"ad c #1c2a32",
"cQ c #1c2a33",
".Y c #1d1d1d",
".0 c #1d1d1e",
".X c #1d1e1d",
"ax c #1d1e1e",
"bi c #1d252b",
"a6 c #1d262c",
"be c #1d262d",
"ei c #1d262e",
"dX c #1d272e",
"dK c #1d2830",
"ac c #1d2831",
"c. c #1d2832",
"ae c #1d2931",
"#x c #1d2932",
"by c #1d2a31",
"aG c #1d2a32",
"cR c #1d2a33",
"#5 c #1e1d1d",
"ay c #1e1d1e",
".Z c #1e1e1d",
".W c #1e1e1e",
"cg c #1e272c",
"aM c #1e272d",
"bQ c #1e272e",
"#F c #1e282f",
"dC c #1e2930",
"#z c #1e2931",
"aH c #1e2932",
"a0 c #1e2a31",
"#y c #1e2a32",
"dB c #1e2a33",
".V c #1f1f1f",
"#M c #1f282e",
"a4 c #1f282f",
"am c #1f292e",
"bF c #1f292f",
"eF c #1f2930",
"cF c #1f2931",
"bp c #1f2a31",
"#A c #1f2a32",
"af c #1f2b32",
"dW c #1f2b33",
".U c #202020",
"bR c #20292f",
"cd c #202930",
"cw c #202931",
"b1 c #202a30",
"ai c #202a31",
"ak c #202a32",
"aj c #202b32",
"a1 c #202b33",
".T c #212121",
"cN c #212a30",
"#L c #212a31",
"#K c #212b31",
"#G c #212b32",
"a5 c #212b33",
"al c #212c32",
"aL c #212c33",
"dZ c #212c34",
".S c #222222",
"#H c #222b32",
"cx c #222b33",
"#I c #222c32",
"#J c #222c33",
"ce c #222d33",
".R c #232323",
"cf c #232d33",
"cS c #232d34",
"dI c #232e35",
".Q c #242424",
".P c #242425",
".N c #242524",
".M c #242525",
".O c #252424",
"#4 c #252425",
"b. c #252524",
".L c #252525",
".K c #262626",
".G c #272727",
".E c #272728",
".H c #272827",
"aw c #272828",
".J c #282727",
".F c #282728",
".I c #282827",
".D c #282828",
".C c #292929",
".B c #2a2a2a",
".x c #2b2b2b",
".A c #2b2b2c",
".z c #2b2c2b",
"#3 c #2b2c2c",
"av c #2c2b2b",
"bv c #2c2b2c",
".y c #2c2c2b",
".w c #2c2c2c",
".v c #2d2d2d",
".r c #2e2e2e",
".u c #2e2e2f",
"bu c #2e2f2e",
"#2 c #2e2f2f",
".t c #2f2e2e",
".s c #2f2e2f",
"au c #2f2f2e",
".q c #2f2f2f",
".p c #303030",
".o c #313131",
".m c #313132",
"aT c #313231",
"at c #313232",
".n c #323131",
"#1 c #323132",
"#0 c #323231",
".l c #323232",
".k c #333333",
".j c #343434",
".i c #353535",
"as c #353536",
".h c #353635",
"#Y c #353636",
".g c #363535",
"aS c #363536",
"#Z c #363635",
".f c #363636",
".e c #373737",
".c c #383838",
".d c #383839",
"#W c #383938",
".a c #383939",
"aR c #393838",
"#X c #393839",
".b c #393938",
".# c #393939",
"Qt c #3a3a3a",
"Qt.#Qt.#.#.#.#.#.a.b.c.c.d.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.g.h.g.f.i.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.k.l.l.l.l.l.l.m.n.o.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.r.s.t.r.t.u.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.x.w.y.z.z.A.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.E.F.G.H.I.J.G.G.G.G.G.K.K.K.K.K.K.L.L.L.L.L.M.L.N.O.P.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.X.X.Y.Z.0.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.3.4.3.5.3.3.6.4.4.4.4.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#a#b#b#a#c#d#b#b#b#b#b#e#e#e#e#e#f#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#h#i#i#i#i#j#j#j#j#j#j#j#j#j#j#i#i#k#l#m#n#o#o#o#p#q#r#r#q#s#s#t#u#v#t#t#w#x#x#x#x#x#y#y#z#z#y#A#y#y#y#A#A#A#A#B#C#D#E#F#G#G#G#G#G#G#G#G#H#I#J#G#H#I#I#K#G#G#G#G#G#G#G#G#G#I#G#G#L#M#N#O#P#Q#R#S#T#U#S#V",
"Qt.#.#.#.#.#.#.#.#.c#W#X.a.a.c.c.c.c.e.e.e.e.e.f.f.f.f.f.f.i#Y#Z.h.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l#0.m.o.l#1.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.u.t.t.r.r#2.u.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w.x.w.A#3.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.J.I.H.D.G.G.G.G.G.G.G.K.K.K.K.K.K.L.L.L.L.L.L.Q.Q.P.Q#4.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.0.0#5.Z.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2#6.4.4.4#6.5.4.4.4.4.4.7.7.7.7.7.7.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#####a#a#a#a#a#a#a#c#7#7#a#c#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#i#j#j#j#j#j#j#j#j#i#i#8#9a.a##oaa#o#o#p#r#rab#q#s#s#s#v#s#tac#uad#x#x#x#x#yae#z#z#y#A#y#y#A#A#A#A#Aafag#D#Dahaiaj#Gakaj#G#G#G#H#I#G#G#H#I#I#K#G#G#G#G#I#Jalam#Nanaoapaqar#T#S#S#S#S#S#S#S#D#V",
"QtQt.#.#.#.#.#.##W.c.c.c.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f#Y.i.h.ias.i.i.i.i.i.i.j.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.lat.mat.m.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.t.tau.r.u.r#2.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w#3.yav.z.y.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.D.E.H.Gaw.I.G.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.Q.Q.P.Q.Q#4.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Zax.Y.W#5ay.Y.Y.Y.Y.Y.1.1.1.1.1.1.2.2.2.2az.3.6aA.5.5.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#a#7#b#7aB#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#j#j#j#j#j#j#j#j#i#iaCaDaEaaaa#oaa#o#o#p#r#rab#q#s#s#s#uaFaF#t#waG#x#x#x#x#y#y#zaH#y#A#y#y#A#A#A#A#Aaf#FaI#DaJaKaL#Gakaj#G#G#G#H#I#J#G#H#I#I#G#G#G#I#IaMaNaOaPaQ#D#T#T#T#U#U#T#T#T#T#S#S#V#V#V",
"QtQt.#.#.#.#.#.#.caR.d.c.#.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.h.iasaS#Y.g.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.l.l.l.l.l.l.laTat.o.l.m.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q#2.q.t.uau#2.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.z.w#3.w.yav.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.G.Haw.E.E.E.G.G.G.G.G.K.K.K.K.K.K.L.L.L.L.L.N.N.Q.Q.P.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.X.Y.X.Y.0.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2#6.4.3.6.3az.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#aaBaUaVaV#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#i#j#j#j#j#j#j#h#jaCaWaXaYaZaa#oaa#o#o#p#r#qab#q#s#s#s#s#s#w#t#wadad#x#x#x#y#y#zaH#y#Aa0#y#y#A#A#A#A#Aa1a2a3#D#Ea4a5akaj#G#G#G#H#J#J#G#H#I#I#Iala6a7a8ar#T#T#U#Ua9#U#U#U#T#T#S#S#S#S#V#V#V#V#V",
"Qt.#.#.#.#.#.#.#.c#W.d.c.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f.h.i#Yas.h.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l#1.oaTaT.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.r.r#2.s#2.r.u.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.z.x.z#3.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.H.G.Haw.J.H.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.Lb..O.O.Q.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.W.0ax.Z.X.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.4#6#6.4.3az.4.4.4.4.7.7.7.7.8.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9#.#.#.#.#.#.####"
"#######a#a#a#a#a#a#b#baBaB#b#b#b#b#b#e#e#e#e#e#e#f#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#h#i#i#i#i#i#j#j#j#j#j#i#ib#babbaXaXaaaa#oaa#o#obc#r#qab#q#s#s#s#sbd#t#waG#w#u#x#xaH#y#y#zaH#y#Aa0#y#A#A#A#A#A#A#Abebf#Dbgbh#G#Gaj#G#G#G#H#I#J#G#H#I#Jbibjbkbl#T#U#D#U#U#U#U#T#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.#aR.d.caR.c.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.faS.h.f.gaS.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.l#0.oaT.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.t.u.r.u.t.r.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.wav.x.z.x.z.x.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.I.G.I.G.H.D.G.G.G.G.G.K.K.K.L.K.L.L.L.L.L.Q#4b..P.M.O.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.U.V.V.V.V.V.W.W.W.W.W.X#5.0ax.Y.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.5aA.4.3aA.4.3.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#a#baV#b#7#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#i#j#j#j#j#j#i#h#kbabmaYaYaXaZaa#oaa#o#o#p#r#qab#q#s#sbnbdbn#t#t#wbo#x#x#xaH#y#y#z#z#ya0a0#y#y#A#A#Abp#A#Aaja7#D#DbqbraLak#G#G#G#H#J#G#G#Ja6bsbt#T#U#U#D#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.##W#W.c.#.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.f.h#Z.ias.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l#0.n#1#1.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.qbu.uau.u.ubu.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.wbv.x.x.x.x.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.G.G.D.F.I.G.J.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L.P.P#4.O.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W#5.0.Yay.W.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.5.4.4.3aAaz.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#b#7aUaBaB#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#j#j#j#j#i#h#kbwbxaYaYaYaXaZaa#oaa#o#obc#r#r#r#q#s#s#s#s#t#tacbobyaG#x#x#x#y#y#zaH#y#A#A#y#A#A#A#A#A#A#Aaj#Fbz#D#DbAaLaj#G#G#G#H#I#G#GbB#Ebl#U#D#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.#.c.c.d.d.d.c.c.c.c.c.e.e.e.e.e.f.f.f.f.f.faSaSaS.i.g.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.l.naT#1.m.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.t.u.s.u.u.q.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.w.xav.yavbv.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.F.F.E.H.F.G.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.L.L.Q.N.Q.N.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.V.V.V.V.V.V.V.W.W.W.W.W#5.Z.Y#5ay.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.4.5.3.5.3.5#6.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.####"
"#######a#a#a#a#a#b#d#b#b#baU#b#b#b#b#e#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#j#j#j#j#gbCapbDbmaXaYaXaXaZaa#oaa#o#o#p#q#qab#q#s#s#saFbnaF#waebo#x#x#x#x#yae#zaH#y#Aaf#y#A#A#A#Aaf#A#Aaka5bEar#DbgbBa5#G#G#G#H#JbFaOar#D#D#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.c.c.d.c.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.h.f.g.has.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.oataT.m.m.o.o.o.o.o.p.p.p.p.p.p.q.q.q.qbubu.t.r.r.q.sbu.r.r.r.v.v.v.v.v.v.w.w.w.w.w.w.x.xav.A.A.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.D.J.G.I.H.G.J.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L#4.Q.L.O.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W#5.Y.X.Y.Y#5.W.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.2.2az.2.5.3.4.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.####"
"#####a#a#a#a#a#a#a#a#d#a#b#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#i#j#jbG#j#lbHbIbmaXaYaYaXaZaabJbK#o#obc#q#q#r#q#s#s#saFbd#tacaebo#x#x#x#x#y#y#zaH#y#Aa0#y#A#A#A#A#A#A#Aakaj#OaJ#D#DbLa4#G#G#G#J#MbM#U#Ubgbg#U#DbgbgbNbg#D#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.##W.c#W#W.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f.g.gas.i.i.i.i.i.i.i.i.i.j.j.j.k.k.k.k.k.k.l.l.l.l.l.l.l.n.m.naT.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.q.t.r.ubu.tbu.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w#3#3.A.x#3.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.H.G.G.J.D.G.G.G.G.G.G.G.G.K.K.K.L.L.L.L.L.L.L.L.N#4.O.O.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Y.Y.Y.0.Y.X.Y.Y.Y.Y.1.1.1.1.1.1.2.2.2.2.6.3.3.2.3aA.5.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#######a#a#a#a#aaB#7aU#caB#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#j#j#i#hb#bObPbPbmaXaYaXaXaZaa#oaa#o#o#p#r#qab#q#s#s#s#s#vaFaFaebo#x#x#x#x#y#y#zaH#y#Aa0#y#A#A#A#A#A#A#AakakbQbf#D#D#D#P#J#G#JbRbS#D#UblarbtbTa3bUbV#T#T#Dbgbgbg#D#U#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.#.caRaRaR.d.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.gas.g.h.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.k.l.l.l.l.l.l.l.oatat.m.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.s.s.r.rau.t.r.r.r.r.v.v.v.v.v.v.w.w.w.w.w.x#3.A.x#3.x.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.F.G.Gaw.J.G.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L.Q.M.Q.M.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.0.Y.Yay#5.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.4.5.5#6.4.3.6.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#c#baV#c#b#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#i#j#j#hbWbXbYbPbPbmbZaYaZaXaZaab0aa#o#o#p#r#r#r#q#s#s#s#u#v#t#tbo#w#x#x#x#x#yae#zaH#y#A#A#y#A#A#A#Aaf#A#AakakaibM#D#D#D#Eb1#Gb1b2bqbAb3aKb4#Nb5b6b4#O#Pb2aPa3#D#D#Db7#U#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.b.#.c.d.d.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f#Z.g.i.i.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.k.l.l.l.l.l#1at.m.o.m.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.qbuaubu.tbu.r.r.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w#3bv.A.z.x.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.Jawaw.I.E.E.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.M.P.M.N.O.Q.Q.Q.Q.R.R.R.R.R.R.R.R.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.Y.W.Y#5.W.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.4aA.5.4#6.5.6.4.4.4.4.7.7.7.7.7.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.###.##"
"#######a#a#a#a#a#a#aaVaV#b#d#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#j#i#i#g#kb8bPbPbPbmb9aYaXaXaZaab0aa#o#obc#q#q#r#q#s#s#s#vbnaFc.#wby#x#x#x#x#yae#zaH#ya0a0#y#y#A#A#Aaf#A#Aakakajc##D#D#D#Dcaa5cbcccd#Jce#J#I#I#G#G#I#I#Jcfalcgchcibt#Tcj#D#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.c#W#W.a.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f#Y.i.i.h#Y.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.k.l.l.l.l.l.l.mataT#0.m.o.o.o.o.p.p.p.p.p.p.p.q.q.q.qau.tbu.q.s#2.r.r.r.r.r.r.v.v.v.v.w.w.w.w.w.wavbv.zav.x.x.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.D.D.J.D.G.G.H.G.G.G.G.G.K.K.K.K.L.L.L.L.L.Lb..O.Q.L.O.O.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.Way.Yay.Y.X.Y.Y.Y.Y.Y.Y.1.1.1.1.1.1.2.2.2az.5#6.5az.5.4az.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#a#a#b#daV#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#j#gbWckclcmbPbPbmbZaYbZaXaZaa#obK#o#o#p#q#rab#q#s#s#saFbd#u#tbo#uaG#x#x#x#yae#zaH#y#A#A#y#y#A#A#A#A#A#Aakaka1cn#D#D#D#Dco#M#G#J#J#J#G#H#I#I#K#G#G#G#G#G#G#I#I#LcpbAcq#Ubg#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.c.c.##W.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.i.g.i#Z.g.i.i.i.i.i.j.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l#1aT.oat.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.r.s.r.ubu.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.wav.xav.x.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.H.F.H.G.E.I.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.N.Q.N.O.P.Q.Q.Q.Q.R.Q.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Y.0ax#5.Z.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.4.6.3#6.5.4.4.4.4.4.4.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#######a#a#a#a#a#a#aaVaV#daU#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#i#hb#crcscmbPbPbmbZaYaYaXaZaab0aa#o#obc#r#rab#q#s#s#sbn#w#w#w#w#xaG#x#x#x#y#y#zaH#y#A#y#y#A#A#A#Abp#A#Aakakajc##D#D#D#Darct#J#H#I#G#G#G#I#I#K#G#G#G#G#G#G#G#J#Kb5bhbU#S#Tbg#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.##W.c#WaR.c.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.iaS.gaS.i.i.i.i.i.i.i.j.j.j.j.j.j.k.k.k.l.l.l.l.l.l.l.oaT#1#1.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.qbu.r.t.q.u.r.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.z.A.z.A.x.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.Daw.Eaw.H.G.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L#4.Q.P.N.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.X.0axay.X.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.3.3.5az.2.4.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.######"
"#######a#a#a#a#a#a#c#b#baV#7#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#h#i#i#i#i#i#icucvcscmbPbPbmbZaYaXaXaZaa#obK#o#o#p#r#rab#q#s#s#s#s#v#t#t#wbo#x#x#x#x#yae#zaH#y#Aa0#y#A#A#Aafaf#A#AakakcwbA#D#D#D#D#Dcucx#H#I#J#G#G#I#I#K#G#G#G#G#I#IbRccbMaQcq#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.##X#W.d.#.c.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.i.i.g.i.g.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l#1.o#1.m.n.n.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.r.u#2.r#2.r.t.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.A.x.x.zav.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.D.Faw.G.G.F.G.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.L.Ob..P.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.Way.Z.0ay.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2#6az.5.6#6.4.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#a#b#7aUaBaU#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#g#kcyczcscmcmbPbmaXaYaYaXaaaa#oaa#o#o#p#q#r#r#q#s#s#sbn#s#w#t#x#w#u#x#x#x#yae#zaH#y#Aa0#y#A#A#A#A#A#A#AakajcAbq#D#D#D#D#Darb6#J#J#G#G#H#I#I#G#G#G#I#IbicBcC#R#T#U#T#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.#.c.b.b.#.c.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.i.i.has.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.laT#0.m.n.m.o.o.o.o.o.p.p.p.p.p.p.p.p.q.q.qbu.q.r.u.u.r.r.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w.y.z.x.x.x.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.J.G.H.I.J.H.H.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L.Qb..P.O.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.W.W.Z#5.Yax.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.6.5.5.4aA.5.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#a#caV#d#b#c#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#ibGaCcDczcscmbPbPbmaXaYaXaXaZaa#oaa#o#o#p#q#r#r#q#s#s#sbdbd#t#u#w#waG#x#x#x#yae#zaH#y#A#A#y#A#A#A#A#A#A#AakakbAbg#D#D#D#D#D#Dbj#G#J#G#G#H#I#I#Gal#K#NaObq#U#U#U#U#U#T#T#T#T#T#S#S#S#S#S#V#V#V#V",
"Qt.#.#.#.#.#.#.#.c#WaR.c#W.b.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.g.g.g.h.i.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.n.naTaT#1.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.t.rbu.r.r.r.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.wbv.Aav#3.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.H.G.I.I.J.J.J.G.G.G.G.K.K.K.K.L.L.L.L.L.L.L.L.L.P.O.Q.N.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.X.X.Y.0.Y.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2az.4.5.5.3.6.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#c#b#caV#caV#b#b#b#b#e#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#g#jbXbYcEcscmbPbPbmaXaYaZaXaZaa#oaa#o#o#p#q#rab#q#s#s#s#sbn#t#waeae#x#x#x#x#y#y#zaH#y#Aaf#y#y#A#A#Aaf#A#AcFcG#D#D#D#D#D#D#D#DcH#Mcx#G#G#G#Iceb1cI#QbU#T#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#S#V#V#V#V",
"Qt.#.#.#.#.#.#.#.c.caR.c.c.c.c.c.c.e.e.e.e.e.e.e.e.f.f.f.f.g.h.g#Z.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.m#0.oaT#0.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.s#2.r#2bu.rau.r.r.r.r.v.v.v.v.v.v.w.w.w.w.w.w#3.x#3avav.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.D.F.F.G.H.E.E.G.G.G.G.K.K.K.K.L.K.L.L.L.L.L.L.O#4.P.P.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.0ay.0#5.Y.Y.Y.Y.Y.Y.1.1.1.1.1.1.2.2.2.2.2.4.4.5.5.4az.5.4.4.4.4.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.######"
"#######a#a#a#a#a#7#a#b#c#7#b#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#fbCcJcKcEcscmcmbPbmbZaYaYaXaZaa#oaa#o#o#p#q#qab#q#s#s#s#u#s#u#wad#xaG#x#x#x#y#y#zaH#y#Aa0#y#y#A#A#Aaf#AajcLcM#D#D#D#D#D#D#D#Dbtb4#J#J#G#IcNcIa8a3#T#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#S#V#V#V#V",
"Qt.#.#.#.#.#.#.#.#.c.c.d.c.c.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.i.i.g.i.i.i.i.i.i.i.i.j.j.j.k.k.k.k.k.l.l.l.l.l.l.l.lat.oatat.o.o.o.o.p.p.p.p.p.p.p.q.q.q.qau.s.r.s.tbu.u.u.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.x.z.A.x.x.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.D.E.Haw.I.J.G.G.G.G.G.K.K.K.K.K.K.L.L.L.L.L.L.O#4#4.P.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.0.Y.Y.Y.X.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.5.6.5.2.4.5.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#a#c#c#baU#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#gcOcPcEcEcscmcmbPbmbZaYaYaXaaaabJaa#o#o#p#r#q#r#rbn#u#ucQcR#u#ubo#u#u#x#x#x#yae#zaH#y#Aaf#y#A#A#A#Aaf#Aa7bg#D#D#D#D#D#D#D#D#D#Db3cS#Jb1#Ocu#D#T#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.#.a#W.baR.c.c.c.c.e.e.e.e.e.e.f.f.f.fas.gas.i.g.f.i.i.i.i.i.i.i.j.j.j.k.k.k.k.k.k.l.l.l.l.l.l.m.o.m.n.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.r.u#2.r.t.u.rau.r.r.r.r.v.v.v.v.w.w.w.w.w.w.wav.z.z.A.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.C.D.D.D.G.Daw.F.E.I.Gaw.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L.Q.Q.Pb..Q.Q.Q.Q.R.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.W.Y.Yay.X.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.2az.6#6.3.3.2.4.4.4.4.4.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#baB#d#caV#b#b#b#b#e#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#h#i#i#h#hcTcDcEcEcscmbPbPbmaXaYaXaXaZaab0aa#o#obc#r#qcUcVcWcXcYcZc0c1c2c3#u#x#x#x#x#y#y#zaH#y#Aa0#y#A#A#A#Aa1c4bq#D#D#D#D#D#D#D#D#D#D#Dc5cec6bXbt#U#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V#V",
"QtQtQt.#.#.#.#.#.#.c.#aR.c.c.c.c.c.e.e.e.e.e.e.e.e.f.f.f.i.f#Z.h#Z.i.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l.l.o#0#0.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.s.r.sbubu.rbu.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.ybv.y.z#3.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.I.Jaw.J.D.D.G.G.G.G.G.K.K.K.K.K.K.L.L.L.L.L.P.P.N.O#4.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Y.0.Xax.Y.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.5.3az.5.4.3.4.4.4.4.4.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.####"
"#######a#a#a#a#a#a#a#caU#b#7#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#h#hcHc7cEcEcscmbPbPbmaXaYaXaXaZaa#oaaa##oc8c9d.d#dadbdbdbdbdbdbdcapddde#x#xaH#yae#z#z#y#Aa0#y#A#A#AafbpcH#D#D#D#D#D#D#D#D#D#D#D#D#Ec5a3#U#U#D#D#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQtQt.#.#.#.#.#.daR.#.daR.c.c.c.c.c.e.e.e.e.e.e.f.f.f.faSaSaS.i#Z.f.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l#0.oat.m.o.o.o.o.o.o.p.p.p.p.p.q.q.q.q.q.q.q.rbu.rbu.s.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.x.zav.A.x.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.Daw.E.F.J.E.H.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L#4.O.P.O.Q.Q.Q.Q.Q.R.Q.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.W.Z.Y.Yax.0.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.5.5aA.4.6.4.4.4.4.4.4.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#aaV#c#baBaB#b#b#b#b#b#e#e#e#e#e#e#f#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#h#icudfcEcEcscmbPbPbmaXaYaXaXaZaa#o#ndgbw#l#8dhdbdbdbdbdbdbdbdbdbdbdhdic#dj#y#yae#z#z#y#A#A#y#A#A#Aa1dkbg#D#D#D#D#D#D#D#D#D#D#D#D#D#Dbg#D#Dbgbgbg#D#U#D#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.b.a#X.#.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f.h.i.h.i.i.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.n.n.o.laT.o.o.o.o.p.p.p.p.p.p.p.q.q.q.qbu.r.qbuau.s.r.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w.z.z.Aav.x.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.G.J.F.G.Haw.E.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L.Q.N.P.M.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.U.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W#5axax.Y.0#5.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.5aA.3.3.4.5#6.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#daV#7aV#d#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#idl#9dmcEcEcscmbPbPbmaXaYaXaXaZaadnbw#8dhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdbdbd#do#yae#zaH#y#A#y#y#A#A#Aafbj#D#D#D#D#D#D#D#D#D#D#D#D#D#D#D#Dbgbg#D#U#D#D#Dbg#Dbg#D#U#U#U#U#T#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.daR.caR#X.c.c.c.c.e.e.e.e.e.e.e.f.f.f.fas.i.g.h.h.i.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.o.o#0aT.n.o.o.o.o.o.p.p.p.p.p.p.q.p.q.q.q.q.ubu.tbu.rbubu.r.r.r.v.v.v.v.v.w.w.w.w.w.w.w.A.A.x.A.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.Haw.F.E.G.G.G.G.G.G.G.G.K.K.K.K.K.L.L.L.L.Lb..O.O.M.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.Wax#5ax#5.Y.Y.Y.Y.Y.Y.1.Y.1.1.1.2.2.2.2.2.2.4.3.4.4.3az.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.#.##"
"#######a#a#a#a#a#a#b#d#7#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#ibWdpczcEcEcsbPbPbPbmbZaYaXaXdqdrd#dhdhdhdhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdbdbapdsae#zaH#y#A#y#y#A#A#AbpbM#D#D#D#D#D#D#D#D#D#D#D#D#DardtbkcHbXbAbAapdudvbT#D#U#Db7#D#U#U#U#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.d.c.c.d.b.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.g.h.i.i.g.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l#1.oaT.n.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.q.s.q#2.rbu.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.w.A.xbvav.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.G.F.G.J.Eaw.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.N.P.N.P.O.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.S.T.T.T.T.T.T.T.T.T.U.U.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.W.X.0ay.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2az.4az.4.5.4.4.4.4.4.7.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#baU#d#d#d#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#h#i#i#ibCdwcKcPdxdydzbPbPbmbZaYaYaXdAdc#8dhdhdhdhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdbdbdbcLdB#zaH#y#Aa0#y#y#A#AdCap#D#D#D#D#D#D#D#D#D#D#D#D#Dap#Mb1#G#I#Jce#I#Gb1a6dD#U#T#D#D#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.a.c.a.c.c.c.c.c.e.c.e.e.e.e.e.f.f.f.f.f.h#Yasas#Z.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.oat.m.m.m.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.q.u#2.r.u.r.s.r.r.r.r.r.v.v.v.v.w.w.w.w.w.wbv.z.x.A.x.A.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.Gaw.H.E.I.I.G.G.G.G.G.K.K.K.K.K.K.L.L.L.L.L.N.Q.M.O.P.N.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.V.W.W.W.Yax.X.Y.W.X.Y.Y.Y.Y.Y.Y.1.1.1.1.1.1.2.2.2.2aA.6#6.4.3.4.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#######a#a#a#a#a#aaB#d#7#d#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#ibCdEdFb##8#kbwclbPbmaXaYaZdGdi#8dh#8dhdhdhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdbdbdbbSae#zaH#y#Aa0#y#A#A#AdCap#D#D#D#D#D#D#D#D#D#D#D#D#DdHdI#G#G#H#I#I#Gal#KctbT#Ubg#D#U#U#U#T#T#T#T#S#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.d.d#WaR.c.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.iaS.g.f.g.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.l.l.l.l.l.l.lat#1.o.o.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.s.r.sauau.r.r.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.z.z#3av.x.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.G.Gaw.H.G.J.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.P.O#4.N.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Wayax.Yay.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.6.4.6.3aA.6.4.4.4.4.7.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#####a#a#a#a#a#a#a#d#d#d#b#7#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#kdxd##8#8#8#8dybIbmaXaYdJdu#8#8dhdhdhdhdhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdbdbdbdcdKaHaH#y#Aa0#y#A#A#AbpdL#D#D#D#D#D#D#D#D#D#D#D#D#Db2cS#J#G#G#I#I#G#Kc#bU#U#D#U#U#U#U#U#T#T#T#T#T#S#S#S#S#V#V#V#V#V",
"Qt.#.#.#.#.#.#.#.c.b.a.b.d.c.c.c.c.c.e.e.e.e.e.f.f.f.f.f.f.i.h.i.i.i.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l#0.l.n.o.m.o.o.o.o.o.p.p.p.p.p.q.q.q.q.q.tbuau.u.r.t.r.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w.z.A.x.x#3.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.Daw.H.D.I.G.F.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L.M.Q.O.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.S.T.T.T.T.T.T.T.T.T.U.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Way.X.0ay.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.2.5.4.4.2aA.4.4.4.4.4.4.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#a#baU#aaV#b#b#b#b#b#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#ibCd##k#8#8#8#8bSclbmaXaYdM#8#8#8#8#8dhdhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbbf#z#zaH#y#A#A#y#A#A#AafdN#D#D#D#D#D#D#D#D#D#D#D#D#DdOcS#J#G#G#I#Ib1bjbl#U#D#U#U#U#U#U#U#T#T#T#T#T#T#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.##W.b.#.c.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.gasas.i#Z.i.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l.l.m#1.m.n.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q#2buau.r.u.r.rbu.r.r.r.v.v.v.v.v.w.w.w.w.w.w.x.x.A.z.xav.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.Daw.J.G.J.F.J.G.G.G.G.G.K.K.K.K.L.L.L.L.L.Lb..L.Q.M.N.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.T.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.V.U.V.V.V.V.V.W.W.W.W.Wayay.Y#5.0.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.4.6.5#6az#6.6.5.4.4.4.4.7.7.7.7.8.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9#.#.#.#.#.#.#.##"
"#####a#a#a#a#a#a#a#b#d#caVaU#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#ibC#k#8#8#8#8#8#ldPdQbZbb#9#8#8#8#8#8dhdhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbbE#y#z#y#y#Aa0#y#y#A#Aa1dR#D#D#D#D#D#D#D#D#D#D#D#D#DcacS#G#G#H#I#IdS#D#D#D#U#U#U#U#U#U#U#T#T#T#T#T#T#S#S#S#V#V#V#V#V",
"Qt.#.#.#.#.#.#.#.c.d.a.c.d.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.f#Y.gas.i.i.i.i.i.i.j.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.laT.o.o.o.o.o.o.o.o.p.p.p.p.p.q.q.q.q.q#2bu.s.r.rau.s.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.w.y.x.Aav.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.G.F.J.F.H.G.H.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.Qb..P.Q.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.Wax.X.X.X.Y.Y.Y.Y.Y.Y.1.Y.1.1.1.1.2.2.2.2.6az#6.4.3#6.4.4.4.4.4.4.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#####a#a#a#a#a#a#a#b#d#7#c#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#ibC#8#8#8#8#8#8aCdTbmbZdUbX#8#8#8#8dhdhdhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdbdbdbdidV#x#zaH#y#Aa0#y#y#A#AdWdXcM#D#D#D#D#D#D#D#D#D#D#DbUcc#J#J#G#H#Icabl#Ubg#U#U#U#U#U#U#U#U#T#T#T#T#T#T#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.a.caR.d.d.c.c.c.c.e.e.e.e.e.e.e.e.f.f.f.f.g.h#YaS.i.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l#0.oaT.m.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.q.q.r.r.rbu#2.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.wbvavbv.zav.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.Gaw.F.I.G.H.E.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.Q.O.Q.Q.M.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.Z.Y#5.Z.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2#6#6.6.6.3.6.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#######a#a#a#a#a#b#a#b#7aV#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#ibW#l#k#8#8#8#8#kdYbmbZbbck#8#8#8#8dhdhdhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdbdbdbcLaH#y#zaH#y#Aa0#y#A#A#A#AdZd0#D#D#D#D#D#D#D#D#D#D#DaQb6#J#G#G#Jd1cM#U#D#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.#.##WaR.d.a.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.iaS.h#Z.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.k.l.l.l.l.l#1aTaT#1.l.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.q.ubu.sbuau.t.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.x.y.A.zav#3.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.D.H.E.I.I.E.E.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.Q.Q.Q.P.N.Q.Q.Q.Q.Q.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.W.Y.Z.Xay.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.2aA.6az.6.2.3.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#caVaV#b#b#b#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#ibWckbS#8#8#8#8b#d2bmbmaXdy#8#8#8#8dhdhdhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdbbzaOac#yae#zaH#y#A#y#y#y#A#Aaf#AdVbq#D#D#D#D#D#D#D#D#D#DcCaM#I#G#IaMdv#Ubg#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#T#S#S#S#S#V#V#V#V",
"QtQtQt.#.#.#.#.#.#.a.d.c#W.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f.f.ias#Z.i.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.laT#0aT.o.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.u#2buau.s.u.r.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.y.y.x.A.x.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.D.I.Iaw.G.J.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L.Q.M.P.N.Q.Q.Q.Q.R.Q.R.R.R.R.R.S.S.S.S.T.S.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.Y#5ay#5.X.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.4.6aA#6aA.5.5.4.4.4.4.7.4.7.7.7.7.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#######a#a#a#a#a#a#b#b#b#7#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#h#i#i#i#ibXdE#k#8#8#8cTd3bmaXaYd4cu#8#8#8dhdhdhdhdhdhdbdbdbdbdbdbdbdbdbdbdbdhd5d6aH#yae#zaH#y#Aa0#y#A#A#A#AbpbpcG#D#D#D#D#D#D#D#D#D#DbM#L#I#G#KaO#Ubg#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#T#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.c.c.c.#.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.fas#Z.h.f.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.maT.o.m.m.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.q.r.t.rbu.r.r.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w.wavbv#3.x.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.D.Haw.G.E.G.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.N.Q.Q.Q.M.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.X.Yaxaxax.Y.Y.Y.Y.Y.Y.1.1.1.1.1.1.2.2.2.6.4.2azaA.3#6.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.####"
"#######a#a#a#a#a#aaVaV#d#d#7#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#h#hd7d8d9dc#8#ke.bPbmaXaYaYe#d##8#8#8dhdhdhdhdhdhdbdbdbdbdbdbdbdbbzbXeaae#x#x#yae#zaH#y#Aa0#y#A#A#A#A#A#AcFbAbg#D#D#D#D#D#D#D#Deb#J#I#Jecbtbgbg#D#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#T#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.##W#X#X.b.c#W.c.c.c.c.c.c.e.e.e.e.e.f.f.f.f.i.g#Z.g.f.i.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.o.o.oat.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.s.t.r.r.s.t.r.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.w.xav#3bv.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.J.D.F.E.H.E.F.G.G.G.G.G.K.K.K.K.K.K.L.L.L.L.L.M.Q.Q#4.L.N.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.V.W.W.W#5.Y#5.Y#5ax.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.6.4.5az.4.3.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.###.##"
"#####a#a#a#a#a#a#b#caVaV#a#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#h#hcHd8cvdEd9edclbPbmaXaYaYaZa.bwdcdhdhdhdhdhdhdhdbdbdbdbdbdbeeefegehaG#x#x#x#y#y#z#z#y#Aa0#y#y#A#A#Abp#AakeiaO#D#D#D#D#D#D#Dbqa4#J#Jb1aP#Ubg#U#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#S#S#S#S#S#V#V#V#V#V",
"QtQtQt.#.#.#.#.#.#.c.#aR.d.c.c.c.c.e.e.e.e.e.e.e.e.f.f.f.f#Z.iasasas.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.n.naTat.m.n.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.qau.s.u.rau.u.r.r.r.r.v.v.v.v.v.v.w.w.w.w.w.w.x#3av.x.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.D.G.E.D.G.E.G.G.G.G.G.G.K.K.K.L.L.L.L.L.L.L.L#4.Q.P.N.Q.Q.Q.Q.R.R.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.0.Y.Y#5.0.X.Y.Y.Y.Y.Y.1.1.1.1.2.1.2.2.2.2.4.5#6azaA.4.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#c#b#daV#c#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#h#i#lejcEcEekcmbPbPbmbZaYaXaXaa#neldxbfdi#8emdhdhdtdhdaend#bEeoep#uaGaG#x#x#x#yae#zaH#y#Aa0#y#y#A#A#Aaf#A#AakcFcncCbt#D#D#DaJa7#J#H#Jct#Ubg#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.d.a.##X#WaR.c.c.c.c.e.e.e.e.e.e.f.f.f.f.fas.fas.g.i.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.o#0.o.o#0aT.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.s.t.tbu.ubu.r.r.r.r.r.v.v.v.v.v.v.w.w.w.w.w.w.A.x.A.A.A.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.D.E.G.F.G.E.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.P.M.O#4.P.Q.Q.Q.Q.Q.Q.R.R.R.R.R.S.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.V.W.W.W.Way.X.0.Z.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2az.3.4.3.3.4.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#b#c#baB#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#hbWeqcEcEcscmbPbPbmbZaYaXaXaZaaa#a#eresdreteueveuewexdeey#xezaFaebo#x#x#x#x#yae#zaH#y#Aa0#y#A#A#A#Aaf#A#Aakaka5dXa7aObXbAcc#L#G#H#MeA#Dbg#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#V#V#V#V#V",
"Qt.#.#.#.#.#.#.#.#.daR.c.c.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.i#Y.f#Y.fas.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.l.l.l.l.l.laT.o#1.oaT.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.t.t.r.ubu.r#2bu.r.r.r.v.v.v.v.v.w.w.w.w.w.w#3.A#3.A.x.A.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.G.H.Jaw.H.Haw.G.G.G.G.G.K.K.K.K.K.K.L.L.L.L.L.Q#4.Q.N.Nb..Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.W.0.Z.Y.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.3#6.3.4.5.6.4.4.4.4.4.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#b#b#b#c#b#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#gbCcJcKcEcscmbPbPbmbZaYaZaXaZaa#oaa#o#pbc#r#rab#q#sbnaF#ubdaF#wad#waG#x#x#x#yae#zaH#y#Aa0#y#A#A#A#Aaf#A#Aakakajaj#G#Gaiai#G#G#G#Jcta3bgbg#D#D#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.a.d.a.b.d.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f.f.h.f.h.h.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.lat.n.m.n#1.o.o.o.o.p.p.p.p.p.p.p.q.q.q.qbu.s.u.qbubu.r.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.wavav.z#3.x.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.E.G.I.G.J.G.F.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.N.P.Qb.#4.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.W.W.W.Wax.W.0#5.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.4.3.4.4.3.5.6.4.4.4.4.7.7.7.7.8.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#a#b#d#b#baB#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#gbCckdfcEcEcmbPbPbmaXaYaZaXaZaabJaa#o#obc#q#r#r#q#s#s#s#tbn#t#tby#u#x#x#x#x#yae#zaH#y#Aa0#y#A#A#A#Aaf#A#Aakakajaj#G#G#Gaj#G#G#G#Gcu#Ub7#D#D#U#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#S#T#S#S#S#V#V#V#V#V",
"Qt.#.#.#.#.#.#.#.#.caR.d.b.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f.i#ZasasaS.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.l.o#0.m#0.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.r.r.s.r.sbu.r.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w.y.y.z.xav.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.Daw.D.H.G.J.H.Gaw.G.G.G.G.K.K.K.K.K.L.L.L.L.L.Lb.b..M.P.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.R.S.S.S.T.S.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Y.0.0ax.0.Z.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.2.4.5.4.4.5.4.3.4.4.4.7.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#a#b#7#baU#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#h#i#ld8cEcEcmcmbPbmaXaYaXaXaZaa#oaa#o#o#p#r#rab#q#s#s#s#uaF#w#t#xae#x#x#x#x#yaeaeaH#ya0a0#y#A#A#A#Aaf#A#Aakakajajaj#G#Gaj#G#G#Jb6#Db7#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.b.caR#W.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f.haS#Yas.g.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l.mataTat.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.r.r.u.r.r.u.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.ybv.A.A.z.x.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.Daw.G.Gaw.G.H.G.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.L.Q#4.Q.O.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Way.Z.0ay.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.4.4aA.4.6az#6.5.4.4.4.7.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#a#baU#7#7#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#gdceBczcscmbPbPbmaXaYbZaXaaaa#obK#o#obc#q#rab#q#s#s#s#uaFaF#tad#xaG#x#x#x#yae#zaH#y#Aa0#y#A#A#A#Aaf#A#Aakakajaj#G#Gakaj#G#Ga5dS#Db7#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.##W.c.d#W.#.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f#Y.h.i.f.ias.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.laT.n.o.o.l.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.s.t.r.s.r#2.u.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.wav.y.xbv.z.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.G.E.J.J.E.E.J.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.L.Qb..N.N.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.V.V.V.V.V.V.V.W.W.W.W.0#5#5.Y.X.0.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.5aAaAaz.4.4#6.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#c#c#c#c#b#b#b#b#b#e#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#h#kcJczcscmbPbPbmbZaYaYaXaZaa#oaa#o#obc#q#q#r#q#s#s#s#vbn#waeae#x#u#x#x#x#y#y#z#z#y#Aaf#y#A#A#A#A#A#A#Aakakajajaj#G#Gaj#G#GbeeCbgbg#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#S#V#V#V#V",
"QtQtQt.#.#.#.#.#.a.b.baR#W.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.i#Y.haSaS.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.k.l.l.l.l.l.l#0.m.oaT.o.o.o.o.o.o.p.p.p.p.p.q.q.q.q.q.q.r.rau.r.t.r.u.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.x.A.A.A.x.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.J.G.Faw.G.G.G.G.G.G.G.K.K.K.K.L.K.L.L.L.L.L.L.O.Q.P.P.N.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.W.W.W.Way.Yay.X.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2aA.6aA#6.6.4.4.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#aaV#caV#baV#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#i#jbSdfcscmbPbPbmbZaYaZaXaZaa#oaa#o#o#p#q#r#r#q#s#sbn#vbn#w#tae#w#x#x#x#x#y#y#zaH#y#Aa0#y#A#A#A#Aaf#A#Aakakajajaj#G#Gaj#Ga5aNa3b7bg#D#D#D#D#D#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.d.#.b#XaR.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.g.h.ias.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.k.l.l.l.l.l.lataT.naT.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.qbu.s.s.u.r#2.r.r.r.r.v.v.v.v.v.v.w.w.w.w.w.Aavbv.wbv.x.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.I.H.G.G.H.H.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.N.N.P.L.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.S.T.T.T.T.T.T.T.T.T.U.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Y.Yay#5#5.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.5.5.4.4.5.2.4.5.4.4.4.4.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#######a#a#a#a#a#aaVaBaU#d#b#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#h#i#ldPczcmbPbPbmbZaYbZaXaZaab0aa#o#o#p#q#r#r#q#s#s#saF#saF#t#uboaG#x#x#x#y#yaeaH#y#Aa0#y#A#A#A#A#A#A#Aakakajajaj#G#Gaj#Ga5bM#Ddb#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.caR.c#W.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f#Y.h#Z.i.i.i.i.i.i.j.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.o#1.maT.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.qbu.r.r.sbu.r.rbu.r.r.r.v.v.v.v.v.v.w.w.w.w.w.w.xav.A.w.x.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.G.I.D.F.G.I.J.G.G.G.G.K.K.K.K.K.L.L.L.L.L.Lb..P.Q.O.P.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.V.W.W.W.Z.W.0.Z.X#5.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.2#6.3.4.4.6.4.4.4.4.4.4.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#7#baU#7aV#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#j#hbCbOcscmbPbPbmaXaYaXaXaZaa#oaa#o#obc#r#r#r#q#s#s#saFaF#u#t#u#w#x#x#x#x#y#y#z#z#y#Aa0#y#y#A#Aafaf#A#Aakakakajaj#G#Gaja5bQcMbgb7#D#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#aR.a.d.b.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f#Z.i.ias.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l#0.m.n#1.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.t.r.u.s.rbu.r.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w.x.z.z.x.A.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.E.J.F.J.H.G.I.J.G.G.G.G.K.K.K.K.K.K.L.L.L.L.L.L.N.O.M.O.Q.Q.Q.Q.Q.Q.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Y#5.X.0.Z.Y.Y.Y.Y.Y.1.1.1.1.1.1.2.2.2.2.4.5aA.4.3aA.3.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#aaVaB#b#c#b#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#j#gbWckclcmbPbPbmaXaYaXaXaZaa#oaa#o#o#p#q#rab#q#s#s#sbn#t#waF#wad#u#x#x#x#y#y#zaH#y#Aa0#y#A#A#Aafaf#A#Aakakajaj#G#G#Gaja5ca#DeDbgbg#D#D#D#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#T#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.a.d.baR.c.d.c.c.c.c.e.e.e.e.e.e.f.f.f.f#Z.ias.h.f.i.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.o.oat.o.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.q#2.u.t.s.rau.r.r.r.r.v.v.v.v.v.v.w.w.w.w.w.w.x#3.A.y.z.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.G.G.I.G.F.G.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.L.L.N.P.O.M.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.Y.Xay.0#5.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2az.4.5.3#6.3.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#####a#a#a#a#a#a#a#b#b#c#a#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#h#i#i#i#i#j#j#ib#dPbPcmbPbmbZaYaYaXaZaa#oaa#o#o#p#r#qab#q#s#seE#vbn#taFae#waG#x#xaH#yae#zaH#y#Aa0#y#A#A#A#Abp#A#Aakakajaj#G#G#GajeFa8bgb7bgbg#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#V#V#V#V#V",
"QtQtQt.#.#.#.#.#.#.daRaR.c#X.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f#Z.i.i.i.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.l#0.o#0.o.m.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.q.s.ubuaubu.t.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.w.A.Aavav#3.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.Haw.E.J.F.I.J.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L#4.Q.Q.O.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.V.W.W.Wax.Y.Y.Z#5#5.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.4.3.6az.6.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#####a#a#a#a#a#a#d#aaB#d#c#b#b#b#b#b#b#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#i#j#g#ke.bPbPbPbmbZaYaXaXaZaa#oaa#o#o#p#r#qab#q#s#s#sbd#s#t#w#u#wad#x#x#x#yae#zaH#y#Aa0#y#A#A#A#Aaf#A#Aakakajajaj#G#G#GeGaJb7b7bg#D#D#D#D#D#D#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V#V",
"Qt.#.#.#.#.#.#.#.##X#W.b.c.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.i.i.h.g.h.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.l#1.oaT.n.l.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.q.r.t.t.r.t.r.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.wav.zbv.w.x.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.F.G.J.E.G.J.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L.P.N.Q.N.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.Xaxay.Yax.Y.Y.Y.Y.Y.Y.1.1.1.1.1.1.2.2.2.2.2.6.3.4.4.6.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#######a#a#a#a#a#a#baV#b#c#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#j#j#i#icubDbPbPbmaXaYaYaXaZaa#oaa#o#obc#r#rab#q#s#s#t#u#v#w#tae#u#u#x#x#x#y#y#zaH#y#Aa0#y#A#A#A#Aaf#A#Aakakakaj#G#G#Ga5ci#Ddbbgbgbg#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.##W.b.c.c.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.h.ias.h.g.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.oaT.o.nat.m.o.o.o.o.o.p.p.p.p.p.q.q.q.q.q.q#2.r.r.r.ubu.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w.A.x.z.z.z.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.J.I.J.H.F.E.G.G.G.G.G.K.K.K.K.K.K.L.L.L.L.L.Nb..M#4.O.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.V.W.W.W.W.0.Y.Y.Y.Y.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.4.6.4.4.3.5.4.6.4.4.4.7.7.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#######a#a#a#a#a#a#baU#a#a#b#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#h#i#i#i#i#i#j#i#hb#crbPbPbmbZaYaYaXaZaa#oaa#o#o#p#r#r#r#q#s#s#tbn#taF#uaGae#x#x#x#x#yae#zaH#y#A#y#y#y#A#A#A#A#A#Aakakajajaj#Ga5brbUb7bgbgbg#D#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V",
"Qt.#.#.#.#.#.#.#.##W.c.d.d.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.faS.haS.f.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.l#0.o.m.nat.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.t#2.u.ubu.t.q.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.A.xav.A.A.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.E.G.H.H.J.F.G.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.O.N.Pb..O.N.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.U.V.V.V.V.V.W.W.W.W.W.Y.0.0ay#5.Y.Y.Y.Y.Y.1.1.1.1.1.1.2.2.2.2.2.3.4.4.3#6.3.4.4.4.4.7.7.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#.#.#.#.######"
"#######a#a#a#a#a#b#d#c#a#caU#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#j#j#j#gbCd9eHbPbmbZaYaZaXaZaa#o#o#o#o#p#q#r#r#q#s#s#sbn#t#tc.adbo#x#x#x#x#y#y#zaH#y#A#y#y#y#A#A#Aaf#A#Aakakajajaj#GeFap#Db7bgbgbg#D#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.caR.caR#W.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.ias.i.g.g.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l.n.n.oaT.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.u.rbubu.tau.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.wbvav.w.xbv.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.E.G.G.E.G.J.G.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.L.P.Q.Nb..Q.Q.Q.Q.Q.R.R.R.R.R.R.S.S.S.S.T.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.W.Yax.Y#5.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.4.5.4#6.4.2.6.4.4.4.7.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#aaBaVaV#d#c#b#b#b#b#b#e#e#e#e#e#f#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#j#j#j#j#i#haCbHbIbmaXaYaYaXaZaa#oaa#o#obc#r#rab#q#s#s#t#s#s#t#t#uaeaG#x#x#x#yae#zaH#y#Aa0#y#A#A#A#A#A#A#Aakakajaj#GaLcGarb7b7bgbgbg#D#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.##WaR.#.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f.i#Y.g.g.g.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.l.o.o.o.o.n.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.t.rau.ubu.r.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w#3.A.z.y#3#3.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.C.D.D.D.D.F.J.F.H.J.H.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.L.Q.Q.N.O.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.V.V.V.V.V.V.W.W.W.W.X.X.Yax.Yay.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.6.4.5.5.4.3.4.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#aaU#d#b#baB#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#i#j#j#j#gbCdxbPbmbZaYaYaXaZaa#oaa#o#o#p#q#qab#q#s#s#taFbn#u#tadbo#x#x#xaH#yae#zaH#ya0a0#y#y#A#A#Aaf#A#AakakajajaLdkbtbgb7bgbgbgbg#D#D#D#D#U#D#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.##W.##X.c.b.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f#YasaS.i.f.i.i.i.i.i.j.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l.nat#1aT.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.r.tau.u.r.u.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.z.x.A.x.y.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.D.G.H.J.F.I.G.G.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.L.L.N.N.O.Q.Q.Q.Q.R.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Y.Z.Y#5.Yax.Y.Y.Y.Y.Y.1.1.1.1.2.1.2.2.2.2.6.4.4.5.5.3.4.4.4.4.7.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#..9#.#.#.#.####"
"#####a#a#a#a#a#a#aaV#d#b#d#b#b#b#b#b#b#e#e#e#e#e#f#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#h#i#i#i#i#i#j#j#j#i#ieIeJbmbZaYaYaXaZaa#oaa#o#o#p#q#r#r#q#s#s#s#ubd#t#wadbyaG#x#xaH#y#y#zaH#y#Aa0#y#y#A#A#A#A#A#Aakakaja5eKcCbgb7bgbgbgbgbg#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#S#V#V#V",
"QtQt.#.#.#.#.#.#.c.d#W.c.c.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.faSas.h.h.h.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.m.n#0.maT.o.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.q#2.u.s.r.u.u.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.wavav.A#3.x.x.x.x.x.x.B.B.B.B.B.C.C.C.C.C.D.D.D.D.Daw.J.H.E.G.I.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.O.N.Q.O.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.U.V.V.V.V.V.V.W.W.W.Wax.X#5#5.Y.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.4.5.5.5.4.4.5.4.4.4.4.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.#.####"
"#######a#a#a#a#a#a#caUaVaV#b#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#j#j#j#j#j#gbCeLbbbZaYaYaXaZaa#oaa#o#o#p#r#r#r#q#s#s#s#u#s#taFaG#w#x#x#xaH#y#y#zaH#y#A#A#y#y#A#A#A#A#A#Aakaka5bQeM#Ddhbgbgbgbgbgbgbg#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#aR.d.c#W.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.f#Y.f.h.g.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.l.l.l.l.l.l.l.o.naT.m.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.q.t.rbu.rau.r.r.r.r.r.r.v.v.v.v.v.w.w.w.w.w.w.x.A#3.xbv.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.J.H.G.Haw.H.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.Qb..Q#4.Q.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.Yay.X.W.X.Y.Y.Y.Y.Y.Y.1.1.1.1.2.2.2.2.2.2.3.6.4.4.4az.4.4.4.4.7.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#a#baVaVaU#b#b#b#b#e#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#i#j#j#j#j#i#ib#eNaYaYaXaXaZaa#oaa#o#o#p#r#r#r#q#s#s#s#s#t#u#tbyad#x#x#x#x#y#y#zaH#y#Aa0#y#y#A#A#Aaf#A#AakajeOcubgdhb7bgbgbgbgbgbgbg#D#D#D#D#D#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#V#V#V#V#V",
"Qt.#.#.#.#.#.#.#.d.c.#.d.c.c.c.c.c.c.e.e.e.e.e.f.f.f.f.f.fas.h.i.g.i.i.i.i.i.i.i.j.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l.oaT.o.m#1.o.o.o.o.o.p.p.p.p.p.p.q.q.q.qbu.rbu.r.s.t.r.r.r.r.r.v.v.v.v.v.v.w.w.w.w.w.w.z#3.x.x.y.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.D.D.D.D.D.E.G.F.E.G.I.F.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.Q.N.P#4.N.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.Y.X.0.0ax.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2.2.5.4.2az.3.4.4.4.4.4.4.7.7.7.7.8.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.####"
"#######a#a#a#a#a#a#a#d#7#d#b#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#f#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#h#i#i#i#i#j#j#j#j#j#j#h#kdxbmaYaXaXaZaa#oaa#o#o#p#r#q#r#q#s#s#s#waFaF#wad#w#x#x#x#x#yae#zaH#ya0a0#y#y#A#A#A#A#A#AajePeMbgdhdbb7bgbgbgbgbgbgbg#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V",
"QtQt.#.#.#.#.#.#.#.caR#W.c.c.c.c.c.e.e.e.e.e.e.e.f.f.f.f.fas.i#Y.i.h.i.i.i.i.i.i.i.j.j.j.k.k.k.k.k.k.l.l.l.l.l.l.n.m#0.n.o.o.o.o.o.o.p.p.p.p.p.p.q.q.q.q.q.q.tbubu#2.t.r.r.r.r.r.v.v.v.v.w.w.w.w.w.w.w.x.y.xav.x.x.x.x.x.B.B.B.B.B.B.B.C.C.C.C.C.D.D.D.G.H.G.F.I.J.G.G.G.G.G.G.K.K.K.K.L.L.L.L.L.L.L.Q#4.Q.Q.L.Q.Q.Q.Q.Q.R.R.R.R.R.R.R.S.S.S.S.T.T.T.T.T.T.T.T.T.T.T.T.U.U.U.V.V.V.V.V.V.V.V.W.W.W.W.W.Y#5.Y.Z.Y.Y.Y.Y.Y.1.1.1.1.1.1.1.2.2.2.4azaA.6az.5.3.4.4.4.4.7.4.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#7aV#c#b#7#b#b#b#b#e#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#h#i#i#i#i#i#i#j#j#j#j#j#i#h#leQaYaYaXaZaa#o#o#o#o#p#r#q#r#q#s#s#sbd#t#tacadbo#x#x#x#x#yae#zaH#y#Aa0#y#A#A#Aafbp#AajeR#Ebgdhb7b7b7bgbgbgbgbgbgbg#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#T#S#S#S#S#S#V#V#V#V",
"Qt.#.#.#.#.#.#.#.#.d.baR.d.c.c.c.c.c.e.e.e.e.e.e.f.f.f.f.f.f#ZaS#Y.i.i.i.i.i.i.i.j.j.j.j.k.k.k.k.k.l.l.l.l.l.l#0.o.o#1.naT.o.o.o.o.p.p.p.p.p.p.p.q.q.q.q.q.u#2au.ubu.r.r.r.r.r.v.v.v.v.v.v.w.w.w.w.w.w.y#3.x.A.z.x.x.x.x.x.B.B.B.B.B.B.C.C.C.C.C.D.D.D.D.E.E.F.J.G.E.G.G.G.G.G.K.K.K.K.K.L.L.L.L.L.L.P.L.P.M.L.P.Q.Q.Q.Q.Q.R.R.R.R.R.S.S.S.S.S.S.T.T.T.T.T.T.T.T.T.T.U.U.U.U.U.V.V.V.V.V.V.W.W.W.W.W.W.0.Y#5#5.Y.Y.Y.Y.Y.Y.1.1.1.1.1.2.2.2.2aA.4.6.5.4az.4.4.4.4.4.7.7.7.7.7.7.8.8.8.8.8.8.9.9.9.9.9.9.9.9.9.9.9#.#.#.#.#.####"
"#####a#a#a#a#a#a#a#b#c#7#7#a#b#b#b#b#b#e#e#e#e#e#e#f#f#f#f#g#g#g#g#g#g#g#g#g#g#g#g#h#h#h#h#i#i#i#i#i#j#j#j#j#j#j#j#gbC#QeSaXaXaZaa#obK#o#o#p#r#q#r#q#s#s#t#s#s#w#uae#u#x#x#xaH#y#y#zaH#y#Aa0#y#A#A#A#Aaf#AeTdtbgdhdbb7b7bgbgbgbgbgbgbg#D#D#D#D#D#U#U#U#U#U#U#U#U#U#U#U#U#T#T#T#T#S#S#S#S#S#V#V#V#V"};


/*
 *  Constructs a ConnectDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConnectDialog::ConnectDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ),
      image0( (const char **) image0_data ),
      image1( (const char **) image1_data )
{
    if ( !name )
	setName( "ConnectDialog" );
    ConnectDialogLayout = new QGridLayout( this, 1, 1, 0, 0, "ConnectDialogLayout"); 

    pixmapLabel1 = new QLabel( this, "pixmapLabel1" );
    pixmapLabel1->setMinimumSize( QSize( 0, 3 ) );
    pixmapLabel1->setMaximumSize( QSize( 32767, 3 ) );
    pixmapLabel1->setPixmap( image0 );
    pixmapLabel1->setScaledContents( TRUE );

    ConnectDialogLayout->addMultiCellWidget( pixmapLabel1, 1, 1, 0, 7 );
    spacer11 = new QSpacerItem( 190, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ConnectDialogLayout->addMultiCell( spacer11, 9, 9, 0, 1 );
    spacer3_2_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    ConnectDialogLayout->addItem( spacer3_2_2, 2, 2 );
    spacer13_2 = new QSpacerItem( 40, 110, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ConnectDialogLayout->addMultiCell( spacer13_2, 5, 7, 6, 7 );

    buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QHBoxLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    comboBox2 = new QComboBox( FALSE, buttonGroup1, "comboBox2" );
    comboBox2->setAutoCompletion( TRUE );
    comboBox2->setDuplicatesEnabled( FALSE );
    buttonGroup1Layout->addWidget( comboBox2 );

    addressComboBox = new QComboBox( FALSE, buttonGroup1, "addressComboBox" );
    addressComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 4, 0, addressComboBox->sizePolicy().hasHeightForWidth() ) );
    addressComboBox->setEditable( TRUE );
    addressComboBox->setAutoCompletion( TRUE );
    addressComboBox->setDuplicatesEnabled( FALSE );
    buttonGroup1Layout->addWidget( addressComboBox );

    ConnectDialogLayout->addMultiCellWidget( buttonGroup1, 5, 5, 1, 5 );
    spacer13 = new QSpacerItem( 20, 110, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ConnectDialogLayout->addMultiCell( spacer13, 5, 7, 0, 0 );
    spacer3_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    ConnectDialogLayout->addItem( spacer3_3, 6, 3 );
    spacer3_2 = new QSpacerItem( 400, 5, QSizePolicy::Minimum, QSizePolicy::Expanding );
    ConnectDialogLayout->addMultiCell( spacer3_2, 4, 4, 0, 7 );

    layout2 = new QGridLayout( 0, 1, 1, 0, 6, "layout2"); 

    textLabel5 = new QLabel( this, "textLabel5" );

    layout2->addWidget( textLabel5, 1, 0 );

    textLabel4 = new QLabel( this, "textLabel4" );

    layout2->addWidget( textLabel4, 0, 0 );
    spacer1_3_2 = new QSpacerItem( 21, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout2->addItem( spacer1_3_2, 1, 1 );

    lineEdit2 = new QLineEdit( this, "lineEdit2" );
    lineEdit2->setFrameShape( QLineEdit::LineEditPanel );
    lineEdit2->setFrameShadow( QLineEdit::Sunken );
    lineEdit2->setEchoMode( QLineEdit::Password );

    layout2->addWidget( lineEdit2, 1, 2 );

    comboBox3 = new QComboBox( FALSE, this, "comboBox3" );
    comboBox3->setEditable( TRUE );
    comboBox3->setAutoCompletion( TRUE );

    layout2->addWidget( comboBox3, 0, 2 );
    spacer1_3 = new QSpacerItem( 21, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout2->addItem( spacer1_3, 0, 1 );

    ConnectDialogLayout->addMultiCellLayout( layout2, 7, 7, 1, 5 );

    dummyLabel = new QLabel( this, "dummyLabel" );
    dummyLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 1, 0, dummyLabel->sizePolicy().hasHeightForWidth() ) );

    ConnectDialogLayout->addMultiCellWidget( dummyLabel, 9, 9, 2, 3 );

    pushButton1 = new QPushButton( this, "pushButton1" );

    ConnectDialogLayout->addWidget( pushButton1, 9, 4 );

    pushButton2 = new QPushButton( this, "pushButton2" );

    ConnectDialogLayout->addMultiCellWidget( pushButton2, 9, 9, 5, 6 );
    spacer3 = new QSpacerItem( 20, 8, QSizePolicy::Minimum, QSizePolicy::Expanding );
    ConnectDialogLayout->addItem( spacer3, 8, 4 );
    spacer3_4 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    ConnectDialogLayout->addItem( spacer3_4, 10, 4 );
    spacer1_3_3 = new QSpacerItem( 16, 16, QSizePolicy::Fixed, QSizePolicy::Minimum );
    ConnectDialogLayout->addItem( spacer1_3_3, 9, 7 );

    textLabel6 = new QLabel( this, "textLabel6" );

    ConnectDialogLayout->addMultiCellWidget( textLabel6, 3, 3, 0, 5 );

    pixmapLabel3 = new QLabel( this, "pixmapLabel3" );
    pixmapLabel3->setMinimumSize( QSize( 0, 60 ) );
    pixmapLabel3->setMaximumSize( QSize( 32767, 60 ) );
    pixmapLabel3->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QFont pixmapLabel3_font(  pixmapLabel3->font() );
    pixmapLabel3_font.setFamily( "Arial" );
    pixmapLabel3_font.setPointSize( 13 );
    pixmapLabel3_font.setBold( TRUE );
    pixmapLabel3->setFont( pixmapLabel3_font ); 
    pixmapLabel3->setPixmap( image1 );
    pixmapLabel3->setScaledContents( TRUE );

    ConnectDialogLayout->addMultiCellWidget( pixmapLabel3, 0, 0, 0, 7 );
    languageChange();
    resize( QSize(417, 298).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( connectPressed() ) );
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( cancelPressed() ) );

    // tab order
    setTabOrder( addressComboBox, comboBox3 );
    setTabOrder( comboBox3, lineEdit2 );
    setTabOrder( lineEdit2, pushButton1 );
    setTabOrder( pushButton1, pushButton2 );
    setTabOrder( pushButton2, comboBox2 );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ConnectDialog::~ConnectDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ConnectDialog::languageChange()
{
    setCaption( tr( "Connect" ) );
    buttonGroup1->setTitle( tr( "Server type and address" ) );
    comboBox2->clear();
    comboBox2->insertItem( tr( "VMFX network" ) );
    comboBox2->insertItem( tr( "Peer-to-peer" ) );
    comboBox2->insertItem( tr( "Workgroup server" ) );
    addressComboBox->clear();
    addressComboBox->insertItem( tr( "www.vmfx.net" ) );
    textLabel5->setText( tr( "Password:" ) );
    textLabel4->setText( tr( "User name:" ) );
    dummyLabel->setText( QString::null );
    pushButton1->setText( tr( "Connect" ) );
    pushButton2->setText( tr( "Cancel" ) );
    textLabel6->setText( tr( "<p>&nbsp;  &nbsp;  Please enter the Jahshaka server details and your login information.</p>" ) );
    pixmapLabel3->setText( QString::null );
}

