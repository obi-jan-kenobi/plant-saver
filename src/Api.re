open Belt;

exception NoAppId;

type temp = {
  temp: float,
  temp_max: float,
  temp_min: float,
};

type main = {main: temp};

type forecasts = {list: array(main)};

module Decode = {
  let temp = json =>
    Json.Decode.{
      temp: json |> field("temp", float),
      temp_max: json |> field("temp_max", float),
      temp_min: json |> field("temp_min", float),
    };
  let main = json => Json.Decode.{main: json |> field("main", temp)};
  let forecasts = json =>
    Json.Decode.{list: json |> field("list", array(main))};
};

let decode = data => data |> Json.parseOrRaise |> Decode.forecasts;

let forecast = (plant: Plant.plant) =>
  switch (Js.Dict.get(Node.Process.process##env, "APPID")) {
  | Some(appId) =>
    Fetch.fetch(
      "http://samples.openweathermap.org/data/2.5/forecast?zip="
      ++ Plant.zipToString(plant.location.zip)
      ++ ","
      ++ Plant.countryToString(plant.location.country)
      ++ "&appid="
      ++ appId,
    )
    |> Js.Promise.then_(Fetch.Response.text)
    |> Js.Promise.then_(data => data |> decode |> Js.Promise.resolve)
  | None => Js.Promise.reject(raise(NoAppId))
  };

let tomorrow = (forecasts: forecasts) => forecasts.list[1];
