// Generated by BUCKLESCRIPT VERSION 3.1.5, PLEASE EDIT WITH CARE
'use strict';

var Json = require("bs-json/src/Json.js");
var Json_decode = require("bs-json/src/Json_decode.js");
var Js_primitive = require("bs-platform/lib/js/js_primitive.js");
var Plant$PlantSaver = require("./Plant.bs.js");

function temp(json) {
  return /* record */[
          /* temp */Json_decode.field("temp", Json_decode.$$float, json),
          /* temp_max */Json_decode.field("temp_max", Json_decode.$$float, json),
          /* temp_min */Json_decode.field("temp_min", Json_decode.$$float, json)
        ];
}

function forecast(json) {
  return /* record */[
          /* main */Json_decode.field("main", temp, json),
          /* dt */Json_decode.field("dt", Json_decode.$$int, json),
          /* dt_txt */Json_decode.field("dt_txt", Json_decode.string, json)
        ];
}

function forecasts(json) {
  return /* record */[/* list */Json_decode.field("list", (function (param) {
                  return Json_decode.array(forecast, param);
                }), json)];
}

function forecast$1(appid, plant) {
  return fetch("http://api.openweathermap.org/data/2.5/forecast?zip=" + (Plant$PlantSaver.zipToString(plant[/* location */2][/* zip */0]) + ("," + (Plant$PlantSaver.countryToString(plant[/* location */2][/* country */1]) + ("&appid=" + appid))))).then((function (prim) {
                  return prim.text();
                })).then((function (data) {
                return Promise.resolve(forecasts(Json.parseOrRaise(data)));
              }));
}

function tomorrowNight(forecasts) {
  return Js_primitive.undefined_to_opt(forecasts[/* list */0].find((function (forecast) {
                    var date = new Date(forecast[/* dt_txt */2]);
                    if (date.getHours() >= 0.0) {
                      return date.getHours() <= 3.0;
                    } else {
                      return false;
                    }
                  })));
}

exports.forecast = forecast$1;
exports.tomorrowNight = tomorrowNight;
/* No side effect */
