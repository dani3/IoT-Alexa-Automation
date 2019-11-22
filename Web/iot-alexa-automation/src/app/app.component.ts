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

  message = "Good morning, Dani!"

  constructor(private http: HttpClient) {
    setInterval(() => {
      this.now = new Date();
    }, 1);

    setInterval(() => {
      let hour = this.now.getHours();
      if (hour >= 6 && hour <= 12) {
        this.message = "Good morning, Dani!"
      } else if (hour > 12 && hour <= 19) {
        this.message = "Good afternoon, Dani!"
      } else {
        this.message = "Good evening, Dani!"
      }

    }, 20 * 60)
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
