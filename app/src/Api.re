exception NoAppId;

type temp = {
  temp: float,
  temp_max: float,
  temp_min: float,
};

type forecast = {
  main: temp,
  dt: int,
  dt_txt: string,
};

type forecasts = {list: array(forecast)};

module Decode = {
  let temp = json =>
    Json.Decode.{
      temp: json |> field("temp", float),
      temp_max: json |> field("temp_max", float),
      temp_min: json |> field("temp_min", float),
    };
  let forecast = json =>
    Json.Decode.{
      main: json |> field("main", temp),
      dt: json |> field("dt", int),
      dt_txt: json |> field("dt_txt", string),
    };
  let forecasts = json =>
    Json.Decode.{list: json |> field("list", array(forecast))};
};

let decode = data => data |> Json.parseOrRaise |> Decode.forecasts;

let forecast = (appid: string, plant: Plant.plant) =>
  Fetch.fetch(
    "http://api.openweathermap.org/data/2.5/forecast?zip="
    ++ Plant.zipToString(plant.location.zip)
    ++ ","
    ++ Plant.countryToString(plant.location.country)
    ++ "&appid="
    ++ appid,
  )
  |> Js.Promise.then_(Fetch.Response.text)
  |> Js.Promise.then_(data => data |> decode |> Js.Promise.resolve);

let isNight = date =>
  Js.Date.getHours(date) >= 0.0 && Js.Date.getHours(date) <= 3.0;

let tomorrowNight = forecasts =>
  Js.Array.find(
    forecast => forecast.dt_txt |> Js.Date.parse |> isNight,
    forecasts.list,
  );
