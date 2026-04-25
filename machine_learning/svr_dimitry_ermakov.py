from sklearn import svm
from sklearn.metrics.pairwise import cosine_similarity
from sklearn.model_selection import KFold, cross_val_predict
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import StandardScaler
import pandas as pd
import numpy as np
import io
from itertools import combinations
from tqdm import tqdm


csv_data: str = """Artist: Song,URL,YT_View_Count,YT_Like_Count,YT_Comment_Count,YT_Duration_Seconds,Exact_Youtube_Publish_Date,Vocalist_chat,Release_Year_chat,BPM_chat,Genre_chat,x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,x21,x22,x23,x24,x25,x26,x27,x28,x29,x30,x31,x32
ACDC: Thunderstruck,https://www.youtube.com/watch?v=lhg9bYNLvOg,473253066,3604438,12757,293,2018-10-25,Male,1990,133,Hard Rock,8,7,4,10,1,2,5,9,7,8,8,7,10,3,8,6,7,10,8,8,8,7,2,7,6,9,6,9,7,5,9,5,2
Bob Marley: Three Little Birds,https://www.youtube.com/watch?v=LanCLS_hIo4,185320322,863350,37502,182,2009-07-11,Male,1977,75,Reggae,9,3,10,10,8,3,2,6,9,7,9,8,5,10,8,6,6,10,5,5,9,9,7,6,6,6,4,8,9,6,5,8,2
The Streets: Fit But You Know It,https://www.youtube.com/watch?v=XEN8uCaBvdg,3010731,34212,251,255,2017-01-25,Male,2004,135,UK Garage,2,7,5,5,2,2,1,2,8,1,3,5,1,2,5,8,2,10,1,5,6,2,1,1,5,1,6,3,6,5,2,1,1
Suki Waterhouse: To Get You,https://www.youtube.com/watch?v=5CPv3ULlWkA,83813,1709,85,245,2024-09-13,Female,2022,140,Indie Pop,4,7,9,6,7,2,5,2,6,7,10,9,6,5,7,4,5,7,3,7,3,5,7,4,2,6,5,6,7,3,6,3,4
Cranberries: Dreams,https://www.youtube.com/watch?v=xc6ulrb2foQ,8412712,66682,2313,255,2017-04-23,Female,1992,129,Alt Rock,8,7,7,6,10,2,4,5,5,7,10,8,6,10,9,3,6,7,4,8,5,4,9,6,9,4,6,7,7,9,8,5,6
Chuck Berry: Johnny B Goode,https://www.youtube.com/watch?v=TReqmJ7iX2M,31612231,344883,377,163,2017-02-26,Male,1958,168,Rock and Roll,8,8,10,8,6,9,7,8,8,5,6,7,7,1,7,8,7,10,7,4,7,8,5,7,7,9,6,8,7,5,8,9,6
Toto: Africa,https://www.youtube.com/watch?v=U1LB_OerHCE,18509960,134327,6460,295,2022-10-09,Male,1982,93,Soft Rock,5,4,6,9,8,4,9,9,9,10,9,7,7,10,8,4,7,8,7,8,8,6,9,7,4,9,6,9,8,7,7,10,3
Grateful Dead: Ripple,https://www.youtube.com/watch?v=5wh7ylJManI,2249587,20135,530,252,2015-03-28,Male,1970,70,Folk Rock,7,10,8,8,8,3,4,5,7,6,9,6,7,8,7,6,7,4,2,6,6,4,2,3,5,5,6,5,4,5,3,1,5
Wagner: Tannhäuser Overture,https://www.youtube.com/watch?v=SRmCEGHt-Qk,15896079,109297,10683,936,2011-03-11,Instrumental,1845,80,Classical,7,8,9,9,8,8,7,5,6,9,5,9,7,3,7,10,9,8,9,6,5,6,9,2,7,8,9,8,2,7,8,9,10
Sublime: Santeria,https://www.youtube.com/watch?v=YkcY8WUwQ-s,168552,1100,24,184,2018-12-04,Male,1996,90,Ska Punk,6,10,6,6,8,3,6,8,4,3,9,9,7,4,7,1,6,10,2,6,7,9,6,4,8,9,4,8,10,8,9,7,2
Chet Faker: Gold,https://www.youtube.com/watch?v=DoJtFI9R4tM,446386,3817,39,286,2014-05-12,Male,2014,110,Electronica,6,5,7,7,5,3,6,5,6,8,4,3,4,3,6,2,5,8,2,2,4,7,2,3,4,1,7,6,3,4,7,10,3
Bill Withers: Ain’t No Sunshine,https://www.youtube.com/watch?v=YuKfiH0Scao,38516451,532206,8554,127,2015-06-13,Male,1971,79,Soul,7,8,8,10,8,3,5,7,8,8,7,9,7,5,8,7,7,8,3,4,5,9,8,6,8,9,9,9,10,5,9,10,7
Prokofiev: Song About Alexander Nevsky,https://www.youtube.com/watch?v=zmsdYLgND9s,10715,111,2,212,2018-07-30,Choral,1938,85,Classical,6,7,8,6,5,6,4,4,7,7,5,7,3,6,6,8,7,5,8,7,4,3,7,3,6,4,7,7,2,5,4,8,8
Irene Cara: What a Feeling,https://www.youtube.com/watch?v=A64KVUfo4ew,199350,2584,125,233,2021-12-30,Female,1983,122,Synthpop,6,5,5,5,4,2,6,6,4,8,5,7,9,4,6,1,4,7,6,6,8,6,10,3,7,3,8,8,5,7,2,6,4
Portishead: The Rip,https://www.youtube.com/watch?v=artaYKPCt_E,414470,3747,177,275,2017-10-07,Female,2008,102,Trip Hop,3,4,8,6,7,2,5,2,5,4,1,8,3,6,6,2,4,2,3,7,3,3,2,2,4,4,6,4,3,5,3,5,6
Dire Straits: Money for Nothing,https://www.youtube.com/watch?v=wTP2RUD_cL0,160019456,1098981,35532,279,2010-02-23,Male,1985,134,Rock,9,10,2,6,8,2,5,10,8,7,3,8,9,8,8,1,6,10,4,7,8,3,2,2,5,7,4,4,9,8,10,8,2
Beck: Loser,https://www.youtube.com/watch?v=0OhUxj8Fewc,5742108,45718,2746,235,2011-02-08,Male,1993,85,Alt Rock,4,9,5,2,8,2,1,2,3,2,2,9,8,10,9,7,3,10,3,6,4,6,1,1,5,2,2,3,7,3,8,5,1
T-Rex: Bang a Gong,https://www.youtube.com/watch?v=TVEhDrJzM8E,24330461,137943,6475,267,2008-07-18,Male,1971,130,Glam Rock,7,6,2,8,7,3,2,6,5,6,3,8,6,8,8,7,7,6,3,6,3,7,3,5,7,5,6,4,6,6,6,1,1
Kate Bush: Running Up that Hill,https://www.youtube.com/watch?v=HYwNM1t9ltI,16863451,181944,3569,301,2023-03-03,Female,1985,108,Art Pop,4,9,5,9,3,2,9,4,9,7,8,3,8,7,5,4,7,9,10,5,6,2,8,5,7,4,7,6,5,7,3,9,7
Bruce Springsteen: I'm Goin' Down,https://www.youtube.com/watch?v=FDePd-mHOL4,14301317,77913,1185,208,2014-11-08,Male,1984,116,Heartland Rock,9,8,6,3,6,4,8,6,4,7,6,6,9,8,6,5,5,6,5,7,5,6,3,9,8,4,7,7,7,7,6,7,4
The Beach Boys: Sloop John B,https://www.youtube.com/watch?v=tLoS8bGr0uo,2691486,17962,702,181,2017-03-02,Male,1966,135,Pop Rock,8,7,5,7,6,5,7,4,7,5,9,9,4,10,6,7,5,9,4,5,5,5,5,6,7,6,7,6,4,4,2,2,3
Bob Dylan and Johnny Cash: Girl from the North Country,https://www.youtube.com/watch?v=Je4Eg77YSSA,7481269,75183,2884,222,2019-03-11,Male,1969,60,Folk,9,7,8,9,10,6,8,8,5,5,6,7,5,7,9,6,6,7,3,6,3,8,4,6,7,4,8,5,5,6,8,3,3
Kanye West: Runaway,https://www.youtube.com/watch?v=2D-JwgfxslM,362983,2831,269,549,2021-07-06,Male,2010,90,Hip Hop,5,7,6,7,8,1,4,5,6,4,8,4,6,2,8,1,6,6,6,5,7,6,1,2,8,8,2,8,10,9,3,8,1
The Beatles: Let It Be,https://www.youtube.com/watch?v=QDYfEBY9NM4,214384753,1594089,24677,244,2018-06-17,Male,1970,72,Pop Rock,9,6,6,10,8,3,9,4,6,10,10,9,3,8,8,3,8,10,5,6,10,8,10,9,9,9,8,10,10,9,10,1,5
Fazerdaze: Cherry Pie,https://www.youtube.com/watch?v=C2JlV5mEBVQ,237454,6394,464,258,2024-09-09,Female,2024,120,Indie Pop,2,8,4,9,3,2,4,3,6,6,8,3,6,10,7,3,6,4,6,7,6,3,10,6,3,6,5,8,4,6,4,5,2
Madonna: Like a Prayer,https://www.youtube.com/watch?v=b_rIk8yKDGI,18581530,208231,3522,343,2015-08-29,Female,1989,111,Pop,5,8,8,7,5,1,8,4,1,9,10,10,8,2,9,4,5,10,9,6,6,6,9,4,6,2,6,7,7,9,7,10,6
The Ronettes: Be My Little Baby,https://www.youtube.com/watch?v=jSPpbOGnFgk,60810548,654288,15842,163,2020-05-26,Female,1963,130,Pop,5,7,7,5,7,2,6,6,4,8,9,6,6,4,8,7,5,8,4,5,4,8,9,8,7,6,8,7,7,8,5,1,5
The Dixie Cups: Iko Iko,https://www.youtube.com/watch?v=KMrrp8kAmbc,649809,10521,0,126,2022-05-19,Female,1965,165,R&B Pop,2,5,7,7,8,3,3,2,6,4,8,8,1,9,5,3,5,7,2,7,8,7,2,7,4,7,4,5,6,3,4,7,4
Nico: These Days,https://www.youtube.com/watch?v=0_z_UEuEMAo,3139851,22805,1294,214,2009-07-03,Female,1967,92,Folk,4,6,4,5,5,3,3,7,4,3,4,7,2,3,5,7,4,3,3,6,2,3,3,5,3,6,6,5,2,5,5,3,4
Al Green: Love and Happiness,https://www.youtube.com/watch?v=DKhxhuwZ-eA,5018197,88082,702,309,2017-05-01,Male,1972,97,Soul,6,7,7,6,9,2,1,8,5,5,4,9,4,7,5,3,5,9,4,7,5,8,7,4,5,8,7,5,7,5,3,10,5
Stevie Wonder / Cypress Hill / Run-DMC (mashup): My Superstitious Brain,https://www.youtube.com/watch?v=1rZ9cqSJgRM,4471402,67496,1734,241,2017-06-06,Male,2010,105,Mashup,8,8,9,7,8,2,4,9,5,7,4,5,7,10,7,4,3,9,5,8,4,7,4,3,4,10,8,5,8,6,8,10,1
Bach: Cello Suite No. 1 in G,https://www.youtube.com/watch?v=1prweT95Mo0,24872236,371989,8912,196,2019-01-14,Instrumental,1717,70,Classical,7,4,10,9,8,9,9,4,9,10,10,9,8,8,9,9,9,9,8,6,9,7,10,2,8,6,9,8,2,7,9,10,9
Dolly Parton: Jolene,https://www.youtube.com/watch?v=Ixrje2rXLMA,106353587,886617,27904,164,2017-06-27,Female,1973,111,Country,7,8,8,9,6,4,10,7,8,9,10,5,9,1,6,10,8,10,6,7,6,9,10,9,6,7,8,7,9,7,4,10,10
Magdalena Bay: Cry for Me,https://www.youtube.com/watch?v=Xcq3xMCcGlI,684635,10925,889,308,2024-08-23,Female,2024,115,Synth Pop,4,9,5,6,4,1,3,6,5,4,1,2,5,4,6,5,4,5,2,7,4,3,9,3,2,6,2,4,5,3,3,6,1
Sam the Sham and the Pharoahs: Little Red Riding Hood,https://www.youtube.com/watch?v=_FA85RO89HA,11330560,97746,4909,168,2008-06-18,Male,1966,135,Garage Rock,5,2,8,9,8,3,3,3,5,5,1,7,2,4,5,8,5,5,3,6,3,5,1,2,6,1,4,4,5,3,4,10,7
Velvet Underground: Oh! Sweet Nuthin',https://www.youtube.com/watch?v=yhwSPRY51gs,9438460,72914,2774,446,2015-10-31,Male,1970,75,Rock,6,7,6,5,6,4,4,5,8,5,4,7,6,6,6,4,5,6,2,6,5,5,7,6,4,6,6,4,4,5,5,4,4
Blondie: Call Me,https://www.youtube.com/watch?v=o3hHsVtqNFQ,119087592,836949,5080,487,2018-08-24,Female,1980,142,New Wave,5,10,6,8,6,2,7,8,6,8,6,6,9,4,8,3,6,9,5,6,5,7,9,5,6,7,7,7,7,7,9,6,5
Lord Huron: The Night We Met,https://www.youtube.com/watch?v=KtlgYxa6BMU,470907998,4295767,133222,209,2015-02-09,Male,2015,87,Indie Folk,8,9,9,8,8,4,7,4,6,9,9,8,9,6,8,3,7,8,5,8,8,6,4,5,7,10,7,10,9,8,6,5,5
New Order: Age of Consent,https://www.youtube.com/watch?v=gG9fEaITgCk,12263305,133048,880,316,2015-09-26,Male,1983,145,Synth Pop,7,8,5,6,7,2,7,4,6,5,8,7,7,7,6,3,4,4,3,5,3,4,4,4,5,8,6,5,5,6,6,2,3
Patsy Cline: Walkin’ After Midnight,https://www.youtube.com/watch?v=bsRNCvHXHHU,16839000,121581,5807,169,2007-05-01,Female,1957,100,Country,4,4,7,9,7,4,4,5,7,7,8,6,3,4,6,5,5,6,3,4,3,6,7,4,5,6,4,4,7,5,7,6,8
"Chappell Roan: Good Luck, Babe!",https://www.youtube.com/watch?v=1RKqOmSkGgM,139509699,1057718,27294,219,2024-04-05,Female,2024,116,Pop,3,7,7,4,6,2,5,4,5,6,8,5,5,4,7,2,4,6,5,6,6,4,10,4,3,6,5,9,7,5,4,5,1
Best Coast: When I'm With You,https://www.youtube.com/watch?v=iDhtJFbwhOQ,319285,5235,191,179,2015-07-15,Female,2010,135,Indie Pop,4,6,4,8,3,1,3,3,1,3,2,8,4,5,8,3,4,5,2,5,5,4,6,5,5,2,6,7,5,5,5,5,2
NMH: In the Aeroplane Over the Sea,https://www.youtube.com/watch?v=1FeD16vu_qQ,8209129,102046,543,203,2020-01-09,Male,1998,145,Indie Folk,8,8,7,5,5,5,6,6,3,4,7,7,8,7,7,7,6,5,3,5,6,6,3,6,7,5,4,7,6,6,4,4,6
Traditional: In the Bleak Midwinter,https://www.youtube.com/watch?v=cBCYZ9jIJkI,1200626,16695,678,230,2022-12-01,Choral,1906,70,Christmas Carol,5,6,7,6,7,10,8,7,6,9,10,4,6,5,7,10,8,7,7,8,10,8,10,9,6,9,10,9,2,7,9,7,9
Traditional: Mo Ghille Mear,https://www.youtube.com/watch?v=zxjvNUNXhkU,20105540,184666,12777,243,2016-02-05,Choral,1900,105,Celtic,7,6,8,7,6,5,6,6,4,9,10,5,6,5,6,10,8,4,8,7,4,3,10,2,6,7,8,8,2,6,6,10,7
A Tribe Called Quest: Can I Kick It?,https://www.youtube.com/watch?v=D-uV8TGjaGU,3515904,60484,2711,254,2020-05-15,Male,1990,96,Hip Hop,6,5,5,9,6,3,1,3,8,2,2,3,2,7,6,2,4,8,4,7,2,3,1,2,8,8,5,2,8,8,5,10,2
LDR: The Greatest,https://www.youtube.com/watch?v=mUWOsiwy_1g,52066,701,20,320,2020-01-29,Female,2019,77,Alt Pop,3,6,7,7,9,2,7,4,2,6,9,7,5,6,7,2,4,8,2,8,3,5,4,4,6,5,3,7,4,4,7,4,4
Pixies: Wave of Mutilation,https://www.youtube.com/watch?v=RuHeAs0rw5M,3561368,41474,320,124,2020-01-02,Male,1989,146,Alt Rock,3,7,4,6,4,1,5,3,7,5,3,6,9,5,6,5,7,3,1,6,2,3,4,4,5,2,3,5,5,4,6,5,1
Air: Cemetary Party,https://www.youtube.com/watch?v=eLxfYF65Eio,79187,594,7,157,2019-06-28,Instrumental,1998,100,Electronic,4,7,2,8,7,2,2,2,5,4,1,2,4,5,4,5,4,6,4,5,3,4,6,1,3,6,6,6,1,4,2,3,4
The Rolling Stones: Out of Time,https://www.youtube.com/watch?v=B_0TOCR_OIk,2857916,23747,1291,204,2019-09-17,Male,1966,130,Rock,5,7,5,8,10,4,5,4,5,5,7,8,7,8,6,4,6,6,1,5,4,5,5,4,4,7,9,5,5,5,4,1,3
Kavinsky: Odd Look,https://www.youtube.com/watch?v=EeEDRqDsj7c,169235,2033,50,291,2013-03-29,Male,2013,105,Synthwave,3,9,1,6,7,1,3,6,6,6,2,2,8,7,3,6,5,3,5,6,5,3,7,6,2,4,6,6,2,2,1,6,1
McIntosh County Singers: Signs of the Judgement,https://www.youtube.com/watch?v=MBvtK49D9dM,124005,2591,13,157,2015-05-19,Choral,1993,115,Gospel,4,6,7,9,7,3,2,4,7,4,1,4,2,4,4,3,6,7,7,4,2,5,1,4,2,4,4,2,5,2,1,10,7
Traditional: Dominus Vobiscum,https://www.youtube.com/watch?v=F6VxR0JxpUQ,28820,448,33,331,2017-08-25,Choral,1000,65,Gregorian,5,8,6,8,8,6,8,9,9,6,10,3,4,3,6,10,9,7,8,3,5,7,7,6,10,8,8,9,2,9,4,3,10
The White Stripes: Dead Leaves and the Dirty Ground,https://www.youtube.com/watch?v=7OyytKqYjkE,14228508,98643,3045,188,2009-05-13,Male,2001,95,Garage Rock,7,5,5,6,5,1,4,5,6,6,6,9,10,9,8,5,3,6,1,5,3,7,2,1,4,4,4,4,6,6,8,3,2
Nena: 99 Luftballoons,https://www.youtube.com/watch?v=Fpu5a0Bl8eY,67349619,618625,0,235,2021-04-24,Female,1983,193,New Wave,5,9,4,5,8,3,6,8,7,8,5,8,4,9,8,7,6,8,5,7,2,6,5,5,6,6,7,6,7,8,7,5,3
Bob Seger: Night Moves,https://www.youtube.com/watch?v=xH7cSSKnkL4,47155516,271373,8814,268,2020-07-10,Male,1976,118,Heartland Rock,8,7,8,6,10,3,7,8,2,7,8,10,9,7,7,4,6,10,2,7,4,9,7,5,7,3,5,6,10,6,7,4,7
Lauryn Hill: Can't Take My Eyes Off You,https://www.youtube.com/watch?v=wVzvXW9bo5U,72050147,517699,9065,224,2014-02-28,Female,1998,100,R&B,6,6,7,7,1,2,6,4,6,8,6,8,5,3,6,1,4,6,4,6,6,8,9,4,7,7,3,8,9,7,3,6,2
La Roux: Bulletproof,https://www.youtube.com/watch?v=MSkOmSK-PNs,745557,7897,277,212,2024-08-03,Female,2009,123,Synthpop,3,5,5,8,4,3,2,4,3,8,7,1,4,9,9,1,4,10,5,7,8,2,6,3,2,6,7,7,10,7,3,1,4
Joanna Newsom: Emily,https://www.youtube.com/watch?v=D1lBOA_8OZ0,625580,9006,30,729,2019-06-03,Female,2006,110,Chamber Folk,6,3,7,6,5,3,2,2,5,2,4,4,7,5,4,5,4,4,2,8,1,2,2,4,6,2,3,5,2,5,2,1,6
Fleet Foxes: Mykonos,https://www.youtube.com/watch?v=dPLaDtvVdaU,6750200,80735,202,276,2019-03-23,Male,2008,105,Indie Folk,8,8,8,6,8,4,6,7,8,10,9,8,9,5,8,4,6,7,4,8,5,7,3,6,6,6,7,6,8,6,7,10,8
The Beastie Boys: Sabotage,https://www.youtube.com/watch?v=onfTL0-A3EA,16498,93,7,183,2022-06-06,Male,1994,168,Rap Rock,5,7,5,9,6,1,6,3,1,7,1,6,10,9,8,4,4,10,6,6,6,4,1,1,5,3,4,4,8,4,7,10,1
Vivaldi: Winter,https://www.youtube.com/watch?v=TZCfydWF48c,25991880,321747,15629,564,2010-09-13,Instrumental,1723,144,Classical,6,4,9,6,8,10,10,7,7,10,9,7,2,9,7,10,9,9,10,9,8,8,10,5,8,9,9,10,1,9,10,10,9
Amy Winehouse: Back to Black,https://www.youtube.com/watch?v=4Q4Gy2Z7xZ0,63783230,695564,1179,242,2018-12-12,Female,2006,123,Soul,7,5,7,9,5,2,3,8,9,6,7,4,6,4,8,4,5,7,2,5,4,5,4,4,5,5,3,5,9,5,3,7,4
Sacred Harp: Nearer My God to Thee,https://www.youtube.com/watch?v=_CTlxFwXHAs,258836,1927,332,332,2008-05-25,Choral,1841,85,Hymn,8,7,7,9,9,5,8,6,4,8,8,4,3,6,7,5,8,5,7,9,4,6,10,5,3,5,4,8,3,2,3,4,2
Leonard Cohen: The Partisan,https://www.youtube.com/watch?v=hs5hOhI4pEE,3076787,22042,875,208,2014-11-19,Male,1969,125,Folk,6,8,8,6,6,4,5,7,7,7,2,8,5,6,6,2,5,5,6,7,1,6,3,3,5,3,5,6,4,7,6,8,6
Muddy Waters: Mannish Boy,https://www.youtube.com/watch?v=bSfqNEvykv0,31227698,384210,11295,330,2016-02-26,Male,1955,60,Blues,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
Arctic Monkeys: Ritz to the Rubble,https://www.youtube.com/watch?v=H8bNHRVwzyA,19985182,140177,687,194,2014-12-13,Male,2006,150,Indie Rock,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
Taylor Swift: Cruel Summer,https://www.youtube.com/watch?v=ic8j13piAhQ,283015197,1649673,43140,180,2019-08-23,Female,2019,170,Pop,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
Tammy Wynette: Stand By Your Man,https://www.youtube.com/watch?v=AM-b8P1yj9w,13846863,121890,6076,161,2015-10-16,Female,1968,80,Country,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
Francois Hardy: La Temps de l'Amour,https://www.youtube.com/watch?v=fCadGsFrj58,11823436,201723,239,145,2017-01-26,Female,1962,145,Ye-ye Pop,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
Abba: Gimme Gimme,https://www.youtube.com/watch?v=-mHMvYn6RVs,13140879,104406,2577,291,2022-10-07,Female,1979,120,Disco,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
First Aid Kit: Emmylou,https://www.youtube.com/watch?v=PC57z-oDPLs,15008513,78404,3046,265,2012-01-06,Female,2012,82,Folk,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
CCR: Have You Ever Seen the Rain,https://www.youtube.com/watch?v=bO28lB1uwp4,19918908,193563,4466,165,2015-03-13,Male,1970,116,Rock,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
Townes Van Zandt: Pancho and Lefty,https://www.youtube.com/watch?v=zprRZ2wFQD4,12055736,93870,6576,294,2012-01-08,Male,1972,100,Country Folk,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
Chris Stapleton: Tennessee Whiskey,https://www.youtube.com/watch?v=4zAThXFOy2c,892332446,4146115,118121,294,2015-04-23,Male,2015,49,Country Soul,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,"""
df: pd.DataFrame = pd.read_csv(io.StringIO(csv_data))
OUTPUT_CSV: str = r"ErmakovDimitry.csv"

df["Vocalist_chat"] = df["Vocalist_chat"].astype("category").cat.codes
df["Genre_chat"] = df["Genre_chat"].astype("category").cat.codes
df["Exact_Youtube_Publish_Date"] = pd.to_datetime(df["Exact_Youtube_Publish_Date"]).map(
    lambda x: x.toordinal()
)
user_columns: list[str] = [f"x{i}" for i in range(33)]

songs_to_predict: pd.DataFrame = df[df["x0"].isna()].copy()
songs_with_known_ratings: pd.DataFrame = df[~df["x0"].isna()].copy()

base_audio_features: list[str] = [
    "YT_View_Count",
    "YT_Duration_Seconds",
    "Exact_Youtube_Publish_Date",
    "Release_Year_chat",
    "Vocalist_chat",
    "Genre_chat",
]
# Not sure if these are useful
candidate_extra_features: list[str] = ["YT_Like_Count", "YT_Comment_Count", "BPM_chat"]

all_feature_combinations: list[list[str]] = [base_audio_features.copy()]
for r in range(1, len(candidate_extra_features) + 1):
    for combo in combinations(candidate_extra_features, r):
        all_feature_combinations.append(base_audio_features + list(combo))

svr_kernels: list[str] = ["rbf", "linear", "poly"]
epsilon_values: np.ndarray = np.linspace(0.1, 2.0, 10)  # 10 values from 0.1 to 2.0

svr_based_out_of_fold_predictions: pd.DataFrame = pd.DataFrame(
    index=songs_with_known_ratings.index, columns=user_columns, dtype=float
)
svr_based_final_predictions: pd.DataFrame = pd.DataFrame(
    index=df.index, columns=user_columns, dtype=float
)

kfolds: KFold = KFold(n_splits=5, shuffle=True, random_state=42)

for user_i in tqdm(user_columns, desc="User"):
    actual_user_ratings: pd.Series = songs_with_known_ratings[user_i]

    min_observed_mse: float = float("inf")
    best_epsilon: float = float("inf")
    best_kernel: str = ""
    optimal_features: list[str] = None

    for feature_set in all_feature_combinations:
        features: pd.DataFrame = songs_with_known_ratings[list(feature_set)]

        feature_scaler: StandardScaler = StandardScaler()
        scaled_features: np.ndarray = feature_scaler.fit_transform(features)

        for kernel_type in svr_kernels:
            for eps in epsilon_values:
                validation_errors: list[float] = []

                for train_index, validation_index in kfolds.split(scaled_features):
                    X_train, X_val = (
                        scaled_features[train_index],
                        scaled_features[validation_index],
                    )
                    y_train, y_val = (
                        actual_user_ratings.iloc[train_index],
                        actual_user_ratings.iloc[validation_index],
                    )

                    temp_model = svm.SVR(kernel=kernel_type, epsilon=eps)
                    temp_model.fit(X_train, y_train)
                    fold_preds = temp_model.predict(X_val)
                    validation_errors.append(mean_squared_error(y_val, fold_preds))

                mean_validation_mse: float = np.mean(validation_errors)

                if mean_validation_mse < min_observed_mse:
                    min_observed_mse = mean_validation_mse
                    best_kernel = kernel_type
                    best_epsilon = eps
                    optimal_features = feature_set

    # Retrain final user model using the best
    print(f"{user_i}: {best_kernel}, {best_epsilon}, {optimal_features}")
    final_features = songs_with_known_ratings[list(optimal_features)]
    scaler = StandardScaler()
    scaled_features = scaler.fit_transform(final_features)

    final_user_model = svm.SVR(kernel=best_kernel, epsilon=best_epsilon)
    final_user_model.fit(scaled_features, actual_user_ratings)

    all_songs_scaled = scaler.transform(df[optimal_features])
    svr_based_final_predictions[user_i] = final_user_model.predict(all_songs_scaled)

    svr_based_out_of_fold_predictions[user_i] = cross_val_predict(
        svm.SVR(kernel=best_kernel, epsilon=best_epsilon),
        scaled_features,
        actual_user_ratings,
        cv=kfolds,
    )

# fill missing ratings with column mean
all_ratings_filled = (
    songs_with_known_ratings[user_columns]
    .copy()
    .apply(lambda col: col.fillna(col.mean()))
)
similarity_matrix: np.ndarray = cosine_similarity(all_ratings_filled.T)

collab_out_of_fold_predictions: pd.DataFrame = pd.DataFrame(
    index=songs_with_known_ratings.index, columns=user_columns
)

for index, user_i in enumerate(user_columns):
    weights: np.ndarray = similarity_matrix[index].copy()
    weights[index] = 0

    weighted_preds = np.dot(svr_based_out_of_fold_predictions.values, weights)
    collab_out_of_fold_predictions[user_i] = weighted_preds / np.sum(np.abs(weights))

# find best alpha
alpha_candidates: np.ndarray = np.linspace(0.0, 1.0, 26)  # steps of 0.04
best_hybrid_alpha: float = 0.0
min_hybrid_mse: float = float("inf")

for alpha in alpha_candidates:
    hybrid_out_of_fold_blend = (alpha * svr_based_out_of_fold_predictions) + (
        (1 - alpha) * collab_out_of_fold_predictions
    )
    total_mse = mean_squared_error(
        songs_with_known_ratings[user_columns].values.flatten(),
        hybrid_out_of_fold_blend.values.flatten(),
    )

    if total_mse < min_hybrid_mse:
        min_hybrid_mse = total_mse
        best_hybrid_alpha = alpha
print(f"Optimal alpha: {best_hybrid_alpha:.2f} with MSE: {min_hybrid_mse:.4f}")

# linear combination of SVR and collaborative predictions
collab_final_predictions: pd.DataFrame = pd.DataFrame(
    index=df.index, columns=user_columns
)
for index, user_i in enumerate(user_columns):
    weights = similarity_matrix[index].copy()
    weights[index] = 0
    weighted_final_preds = np.dot(svr_based_final_predictions.values, weights)
    collab_final_predictions[user_i] = weighted_final_preds / np.sum(np.abs(weights))

final_hybrid_ratings = (best_hybrid_alpha * svr_based_final_predictions) + (
    (1 - best_hybrid_alpha) * collab_final_predictions
)
final_hybrid_ratings = np.clip(np.round(final_hybrid_ratings), 1, 10)

df.loc[df["x0"].isna(), user_columns] = final_hybrid_ratings.loc[
    df["x0"].isna(), user_columns
]

# Export
output_columns = ["Artist: Song", "URL"] + user_columns
df[output_columns].to_csv(OUTPUT_CSV, index=False)
