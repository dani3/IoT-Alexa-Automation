import { Component } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})

export class AppComponent {
  title = 'IoT Alexa Automation';
  now = new Date();

  constructor(private http: HttpClient) {
    setInterval(() => {
      this.now = new Date();
    }, 1);
  }

  toggleFootLamp() {
    this.http.get<any>('http://192.168.1.4:80/toggleLight').subscribe(data => {
      console.log(data);
    });
  }

  toggleAmbilight() {
    this.http.get<any>('http://192.168.1.5:80/toggleLight').subscribe(data => {
      console.log(data);
    });
  }

  toggleAmbientLights() {
    this.http.get<any>('http://192.168.1.5:81/toggleLight').subscribe(data => {
      console.log(data);
    });
  }
}
