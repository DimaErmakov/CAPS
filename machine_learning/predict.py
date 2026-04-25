from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import Ridge
from sklearn.pipeline import Pipeline
import pandas as pd
import io

csv_data: str = """year,country,host,medals_total,GDP_billion_USD,population_million,snowfall_cm,highest_elevation_m,average_height_cm,average_weight_kg,total_events
1924,United States,No,4,87,110,250,6190,169,68,16
1924,Norway,No,17,1.2,2.4,120,2469,168,63,16
1924,Italy,No,0,10,38.8,300,4806,166,62,16
1924,Canada,No,1,4,9.2,400,5959,170,68,16
1924,Austria,No,3,1.5,6.5,300,3798,167,62,16
1928,United States,No,6,97,120,250,6190,170,69,14
1928,Norway,No,15,1.3,2.7,120,2469,169,64,14
1928,Italy,No,0,9,40.5,300,4806,166,63,14
1928,Canada,No,1,4.5,9.6,400,5959,170,68,14
1928,Austria,No,4,1.8,6.6,300,3798,168,63,14
1932,United States,Yes,12,59,125,250,6190,170,70,14
1932,Norway,No,10,1,2.8,120,2469,170,65,14
1932,Italy,No,0,8,41.6,300,4806,167,63,14
1932,Canada,No,7,3.5,10.4,400,5959,171,69,14
1932,Austria,No,2,1.2,6.7,300,3798,169,64,14
1936,United States,No,4,95,128,250,6190,170,71,17
1936,Norway,No,15,1.4,3,120,2469,170,65,17
1936,Italy,No,0,10,43,300,4806,167,64,17
1936,Canada,No,1,5,11,400,5959,171,69,17
1936,Austria,No,4,1.5,6.8,300,3798,169,64,17
1948,United States,No,9,258,146,250,6190,171,72,22
1948,Norway,No,10,2,3.1,120,2469,170,65,22
1948,Italy,No,1,12,46.4,400,4806,168,65,22
1948,Canada,No,3,15,12,400,5959,171,69,22
1948,Austria,No,8,2,6.8,300,3798,170,65,22
1952,United States,No,11,358,157,250,6190,171,73,22
1952,Norway,Yes,16,2.8,3.4,120,2469,171,66,22
1952,Italy,No,2,18,47.5,400,4806,168,65,22
1952,Canada,No,2,25,14,400,5959,171,69,22
1952,Austria,No,8,3,6.9,300,3798,170,65,22
1956,United States,No,7,420,168,250,6190,171,74,24
1956,Norway,No,4,4,3.4,120,2469,171,66,24
1956,Italy,Yes,3,25,48.6,400,4806,168,66,24
1956,Canada,No,3,30,16,400,5959,171,69,24
1956,Austria,No,11,5,7,300,3798,170,65,24
1960,United States,Yes,10,543,180,250,6190,171,75,27
1960,Norway,No,6,5.5,3.6,120,2469,172,67,27
1960,Italy,No,1,41,50.2,400,4806,169,66,27
1960,Canada,No,4,40,17,400,5959,171,69,27
1960,Austria,No,6,8,7,300,3798,170,65,27
1964,United States,No,7,680,191,250,6190,171,76,34
1964,Norway,No,15,7,3.7,120,2469,172,67,34
1964,Italy,No,4,65,51.7,400,4806,169,67,34
1964,Canada,No,3,50,19,400,5959,171,69,34
1964,Austria,Yes,12,10,7.2,300,3798,171,66,34
1968,United States,No,7,940,200,250,6190,172,77,35
1968,Norway,No,14,9,3.8,120,2469,172,68,35
1968,Italy,No,4,90,53,400,4806,170,67,35
1968,Canada,No,3,70,20,400,5959,171,69,35
1968,Austria,No,11,12,7.3,300,3798,171,66,35
1972,United States,No,8,1280,208,250,6190,172,78,35
1972,Norway,No,12,15,3.9,120,2469,173,69,35
1972,Japan,Yes,3,318,107,485,3776,164,58,35
1972,Italy,No,5,150,54.4,400,4806,170,68,35
1972,Canada,No,1,120,22,400,5959,171,69,35
1972,Austria,No,5,25,7.4,300,3798,171,66,35
1976,United States,No,10,1800,218,250,6190,173,79,37
1976,Norway,No,7,25,4,120,2469,173,70,37
1976,Italy,No,4,230,55.7,400,4806,170,68,37
1976,Canada,No,3,200,23,400,5959,172,70,37
1976,Austria,Yes,6,50,7.5,300,3798,171,66,37
1980,United States,Yes,12,2862,227,250,6190,172,78,38
1980,Norway,No,10,35,4.1,120,2469,173,70,38
1980,Italy,No,2,480,56.4,400,4806,171,69,38
1980,Canada,No,2,270,24,400,5959,172,70,38
1980,Austria,No,7,80,7.5,300,3798,171,66,38
1984,United States,No,4,4000,235,250,6190,173,79,39
1984,Norway,No,9,60,4.1,120,2469,173,70,39
1984,Italy,No,2,450,56.5,400,4806,171,69,39
1984,Canada,No,4,350,25,400,5959,172,71,39
1984,Austria,No,1,70,7.5,300,3798,172,67,39
1988,United States,No,6,5200,245,250,6190,173,79,46
1988,Norway,No,5,90,4.2,120,2469,173,70,46
1988,Japan,No,1,3000,122,485,3776,166,60,46
1988,Italy,No,5,860,56.6,400,4806,171,70,46
1988,Canada,Yes,5,594,26,400,5959,172,72,46
1988,Austria,No,10,130,7.6,300,3798,172,68,46
1992,United States,No,11,6500,256,250,250,174,80,57
1992,Norway,No,20,120,4.3,120,2469,173,71,57
1992,Japan,No,7,3900,124,485,3776,167,60,57
1992,Italy,No,14,1300,56.7,400,4806,172,70,57
1992,Germany,No,26,1700,80,250,2962,175,78,57
1992,China,No,3,420,1170,200,8849,165,60,57
1992,Canada,No,7,590,28,400,5959,173,74,57
1992,Austria,No,21,170,7.9,300,3798,172,70,57
1994,United States,No,13,7300,260,250,6190,174,80,61
1994,Norway,Yes,26,130,4.3,120,2469,172,70,61
1994,Japan,No,5,4900,125,485,3776,167,61,61
1994,Italy,No,20,1000,57,400,4806,172,71,61
1994,Germany,No,24,1700,81,250,2962,175,78,61
1994,China,No,3,560,1198,200,8849,166,61,61
1994,Canada,No,13,570,29,400,5959,173,74,61
1994,Austria,No,9,160,8,300,3798,172,70,61
1998,United States,No,13,9000,276,250,6190,174,80,68
1998,Norway,No,25,150,4.5,120,2469,175,76,68
1998,Japan,Yes,10,4000,126,485,3776,168,61,68
1998,Italy,No,10,1200,57,300,4806,172,71,68
1998,Germany,No,29,2200,82,250,2962,176,79,68
1998,China,No,8,1020,1248,200,8849,166,62,68
1998,Canada,No,15,630,30,400,5959,174,75,68
1998,Austria,No,17,170,8.1,300,3798,173,71,68
2002,United States,Yes,34,10929,288,250,6190,173,80,78
2002,Norway,No,25,170,4.5,120,2469,175,76,78
2002,Japan,No,2,4100,127,485,3776,169,62,78
2002,Italy,No,13,1200,57,300,4806,173,72,78
2002,Germany,No,36,2000,82,250,2962,176,79,78
2002,China,No,8,1470,1285,200,8849,167,63,78
2002,Canada,No,17,700,31,400,5959,174,76,78
2002,Austria,No,17,210,8.1,300,3798,173,72,78
2006,United States,No,25,14000,298,250,6190,174,81,84
2006,Norway,No,19,340,4.6,120,2469,177,79,84
2006,Japan,No,1,4600,128,485,3776,170,63,84
2006,Italy,Yes,11,1900,58,300,4806,173,72,84
2006,Germany,No,29,3000,82,250,2962,177,80,84
2006,China,No,11,2750,1314,200,8849,167,63,84
2006,Canada,No,24,1300,32,400,5959,175,78,84
2006,Austria,No,23,320,8.3,300,3798,174,77,84
2010,United States,No,37,15000,309,250,6190,175,81,86
2010,Norway,No,23,430,4.9,120,2469,178,81,86
2010,Japan,No,5,5700,128,485,3776,170,63,86
2010,Italy,No,5,2100,59,300,4806,173,72,86
2010,Germany,No,30,3300,82,250,2962,178,80,86
2010,China,No,11,6000,1341,200,8849,168,64,86
2010,Canada,Yes,26,1611,34,400,5959,175,79,86
2010,Austria,No,17,390,8.4,300,3798,174,77,86
2014,United States,No,28,17000,318,250,6190,175,81,98
2014,Norway,No,27,500,5.1,120,2469,178,81,98
2014,Japan,No,8,4800,127,485,3776,171,64,98
2014,Italy,No,8,2100,60,300,4806,174,73,98
2014,Germany,No,19,3800,81,250,2962,178,80,98
2014,China,No,9,10400,1371,200,8849,168,64,98
2014,Canada,No,25,1800,35,400,5959,175,79,98
2014,Austria,No,17,440,8.5,300,3798,174,77,98
2018,United States,No,23,20500,327,250,6190,175,81,102
2018,Norway,No,39,440,5.3,120,2469,179,82,102
2018,Japan,No,13,5000,126,485,3776,171,64,102
2018,Italy,No,10,2000,60,300,4806,174,73,102
2018,Germany,No,31,4000,83,250,2962,179,81,102
2018,China,No,9,13800,1403,200,8849,169,65,102
2018,Canada,No,29,1700,37,400,5959,175,79,102
2018,Austria,No,14,450,8.8,300,3798,174,77,102
2022,United States,No,25,23000,331,250,6190,175,82,109
2022,Norway,No,37,500,5.4,120,2469,179,82,109
2022,Japan,No,18,4900,125,485,3776,171,64,109
2022,Italy,No,17,2000,59,300,4806,174,73,109
2022,Germany,No,27,4000,83,250,2962,179,81,109
2022,China,Yes,15,17963,1412,200,8849,169,65,109
2022,Canada,No,26,2100,38,400,5959,176,79,109
2022,Austria,No,18,470,8.9,300,3798,174,77,109"""

df: pd.DataFrame = pd.read_csv(io.StringIO(csv_data)).dropna()


df["host"] = df["host"].map({"Yes": 1, "No": 0})

total_possible_medals: int = 116 * 3
df["total_possible_medals"] = df["total_events"] * 3
df["medals_percentage"] = df["medals_total"] / df["total_possible_medals"]


attributes: list[str] = [
    "GDP_billion_USD",
    "population_million",
    "snowfall_cm",
    "highest_elevation_m",
    "host",
    "average_height_cm",
    "average_weight_kg",
]
target: str = "medals_percentage"

X: pd.DataFrame = df[attributes]
y: pd.Series = df[target]

# train_df: pd.DataFrame = df[df["year"] < 2018]
# test_df: pd.DataFrame = df[df["year"] >= 2018]

X_train: pd.DataFrame = df[attributes]
y_train: pd.Series = df[target]
# X_test: pd.DataFrame = test_df[attributes]
# y_test: pd.Series = test_df[target]

model: Pipeline = Pipeline(
    [("scaler", StandardScaler()), ("regressor", Ridge(alpha=1.0))]
)

model.fit(X_train, y_train)

projections_2026: dict[str, list[float | int]] = {
    "Austria": [604.2, 9.22, 0],
    "Canada": [2420.0, 43.1, 0],
    "China": [20650.0, 1412.9, 0],
    "Germany": [5328.0, 84.5, 0],
    "Italy": [2701.0, 58.9, 1],
    "Japan": [4463.0, 122.0, 0],
    "Norway": [547.7, 5.67, 0],
    "United States": [31821.0, 349.0, 0],
}

print("2026 Medal Predictions:")
for country_name in projections_2026.keys():

    latest_data: pd.Series = (
        df[df["country"] == country_name].sort_values(by="year").iloc[-1]
    )
    snowfall: float = latest_data["snowfall_cm"]
    elevation: float = latest_data["highest_elevation_m"]
    height: float = latest_data["average_height_cm"]
    weight: float = latest_data["average_weight_kg"]

    gdp_2026, pop_2026, host_2026 = projections_2026[country_name]

    X_new: pd.DataFrame = pd.DataFrame(
        [[gdp_2026, pop_2026, snowfall, elevation, host_2026, height, weight]],
        columns=attributes,
    )

    predicted_pct: float = float(model.predict(X_new)[0])
    predicted_medals: int = int(round(predicted_pct * total_possible_medals))

    if predicted_medals < 0:
        predicted_medals = 0

    print(f"{country_name}: {predicted_medals}")
