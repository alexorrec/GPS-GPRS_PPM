import { Component } from '@angular/core';
import { ViewChild, ElementRef } from '@angular/core';
import { HttpClient, HttpClientModule } from '@angular/common/http';

declare var google: any;

@Component({
  selector: 'app-tab1',
  templateUrl: 'tab1.page.html',
  styleUrls: ['tab1.page.scss']
})

export class Tab1Page {
  //map obj
  map: any;
  @ViewChild('map', {read: ElementRef, static: false}) mapRef: ElementRef;

  //Useful vars
  infoWindows: any = [];
  markers: any = {};

  constructor(private http: HttpClient, private httpM: HttpClientModule) {}

  //Fetch Data from Server
  async loadLocation(){
    await this.http.get('http://gsmgpsppm.000webhostapp.com/fetch_last_position.php')
    .toPromise().then(data => this.markers = data[0]); 
    console.log(this.markers);
  }

  //View Action
  async ionViewDidEnter() {
    await this.loadLocation();
    this.showMap();
  }

  //Add Marker to map
  addMarkersToMap() {
    let position = new google.maps.LatLng( this.markers.latitude , this.markers.longitude );
    let mapMarker = new google.maps.Marker({
      position: position,
      latitude: this.markers.latitude,
      longitude: this.markers.longitude
    });
    mapMarker.setMap(this.map);
    this.addInfoWindowToMarker(mapMarker);
  }

  //Add small infoWindow
  addInfoWindowToMarker(marker) {
    let infoWindowContent = '<div id="content">' +
                              '<h4 id="firstHeading" class"firstHeading" style="color:black"> Last seen: </h4>' +
                              '<p style="color:black">Latitude: ' + marker.latitude + '</p>' +
                              '<p style="color:black">Longitude: ' + marker.longitude + '</p>' +
                              '<ion-button id="navigate">Navigate</ion-button>' +
                            '</div>';

    let infoWindow = new google.maps.InfoWindow({
      content: infoWindowContent
    });

    marker.addListener('click', () => {
      infoWindow.open(this.map, marker);

      google.maps.event.addListenerOnce(infoWindow, 'domready', () => {
        document.getElementById('navigate').addEventListener('click', () => {
          console.log('navigate button clicked!');
          //Open external Navigation
          window.open('https://www.google.com/maps/dir/?api=1&destination=' + marker.latitude + ',' + marker.longitude);
        });
      });

    });
    this.infoWindows.push(infoWindow);
  }

  //Build centered map using map obj and position fetched
  showMap() {
    const location = new google.maps.LatLng(this.markers.latitude, this.markers.longitude);
    const options = {
      center: location,
      zoom: 14,
      disableDefaultUI: false
    }
    this.map = new google.maps.Map(this.mapRef.nativeElement, options);
    this.addMarkersToMap();
  }

}
