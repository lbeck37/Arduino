//BeckAsyncWebServerHTML.h, 4/24/19a
#pragma once

const char* acAngularTestPagesHTML= R"(
<!DOCTYPE HTML><html>
<!doctype html>
<html>
  <head>
    <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.7.8/angular.min.js"></script>
    <script>
      var BeckApp = angular.module("BeckModule", []);
      BeckApp.controller("BeckController", function ($scope) {
        $scope.Message = "Hello To The Dude's First AngularJS Program";
      });
    </script>
  </head>
  <body>
    <div ng-app>
      <label>Name:</label>
      <input type="text" ng-model="yourName" placeholder="Enter a name here">
      <hr>
      <h1>Hello {{yourName}}!</h1>
    </div>

    <div ng-app= "BeckApp" ng-controller= ”BeckController”>
        <h2>{{Message}}</h2>
    </div>
  </body>
</html> )";


const char* acThermostatTestPagesHTML= R"(
<!-- BeckThermostatTestPagesHTML.h, 4/18/19a -->
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.8.1/css/all.css" integrity="sha384-50oBUHEmvpQ+1lW4y57PTFmhCaXp0ML5d60M1M7uH2+nqUivzIebhndOJK28anvf" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>BIOTA</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
    <span class="dht-labels">Current</span>
    <span id="LastDegF">%TEMPERATURE%</span>
    <sup class="units">&deg;F</sup>
  </p>
  <p>
    <i class="fas fa-tachometer-alt" style="color: Tomato;"></i>
    <span class="dht-labels">Setpoint</span>
    <span id="SetPointDegF">%SET_POINT%</span>
    <sup class="units">&deg;F</sup>
  </p>
  <p>
    <i class="fas fa-stroopwafel fa-spin" style="color: Dodgerblue;"></i>
    <span class="dht-labels">Offpoint</span>
    <span id="TermoOffDegF">%THERMO_OFF%</span>
    <sup class="units">&deg;F</sup>
  </p>
</body>
</html>)";
//ast line.
