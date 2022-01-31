# QT_iCalendar

<div id="top"></div>


[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

<br />
<div align="center">
  <a href="https://github.com/MartenAlan/QT_iCalendar">
    <img src="https://user-images.githubusercontent.com/69800773/151324755-9a54488c-0b88-4b96-9792-c045654975dc.svg" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">QT iCalendar</h3>

  <p align="center">
    Ein Programm zum Erstellen von iCalendar-Dateien.
    <br />
  </p>
</div>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
    </li>
    <li><a href="#features">Features</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

Mit dem Programm QT_iCalendar kann eine iCalendar-Datei erstellt werden. Zusätzlich kann eine Liste von Feiertagen mit in den Kalender exportiert werden. Die iCalendar-Dateien wurden mit dem Validator auf [iCalendar.org](https://icalendar.org/validator.html) getestet. Somit sollten alle Kalenderprogramme die entstandene Datei verarbeiten können.

<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

Diese Frameworks wurden verwendet.

* [QT Creator](https://www.qt.io/product/development-tools)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

Hier kommt hin, wie man das Projekt zum Laufen bekommt.


<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Features

Alle Eingabefelder sind an das iCalendar-Format angelehnt, somit gibt es Verlinkungen zu der jeweiligen Dokumentation auf icalendar.org.

![UI](https://user-images.githubusercontent.com/69800773/151759996-c16f54f9-a0c6-41e6-a84d-0d2ba321927e.png)

1. Die benötigten Elemente: Titel, Start- und Enddatum
2. eine längere Beschreibung
3. Geographischen Elemente: Die [Addresse](https://icalendar.org/iCalendar-RFC-5545/3-8-1-7-location.html) oder der [Längen- und Breitengrad](https://icalendar.org/iCalendar-RFC-5545/3-8-1-6-geographic-position.html).
4. [Wiederholungen](https://icalendar.org/iCalendar-RFC-5545/3-8-5-3-recurrence-rule.html): Das Event kann vom täglichen bis jährlichen Rythmus wiederholt werden. 
5. Ende der Wiederholung: Das Event kann entweder nach einer bestimmten Anzahl enden oder ab einem Enddatum. Außerdem kann ausgewählt werden, dass die Wiederholung des Events nie endet.
6. [Alarm](https://icalendar.org/iCalendar-RFC-5545/3-6-6-alarm-component.html): Ein Alarm, welcher entweder als Sound- oder angezeigte Nachricht verwendet werden kann.
7. [Priorität](https://icalendar.org/iCalendar-RFC-5545/3-8-1-9-priority.html): Möglich von 0 ( sehr unwichtig ) bis 9 ( sehr wichtig ).
8. Feiertage: Hier können die Feiertage mit dem Button "Get" angezeigt werden. Mit der Checkbox kann sich für ein Export entschieden werden.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [ ] Start- und Enddatum hat den Wert des heutigen Datums.
- [ ] Menüleiste mit Optionenfeld
- [ ] individueller Sound und Beschreibung beim Alarm
- [ ] Importieren von ics-Datei zum Bearbeiten
- [ ] Wechsel zwischen deutscher und englischer Übersetzung


<p align="right">(<a href="#top">back to top</a>)</p>





<!-- LICENSE -->
## License

Distributed under the GNU General Public License. See `LICENSE` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

* Project Link: [https://github.com/MartenAlan/QT_iCalendar](https://github.com/MartenAlan/QT_iCalendar)
* Marten : [https://github.com/MartenAlan](https://github.com/MartenAlan)
* Stefan : [https://github.com/stefan-de70](https://github.com/stefan-de70)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [otherneildrew - Best-README-Template](https://github.com/othneildrew/Best-README-Template)

<p align="right">(<a href="#top">back to top</a>)</p>

 

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/MartenAlan/QT_iCalendar.svg?style=for-the-badge
[contributors-url]: https://github.com/MartenAlan/QT_iCalendar/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/MartenAlan/QT_iCalendar.svg?style=for-the-badge
[forks-url]: https://github.com/MartenAlan/QT_iCalendar/network/members
[stars-shield]: https://img.shields.io/github/stars/MartenAlan/QT_iCalendar.svg?style=for-the-badge
[stars-url]: https://github.com/MartenAlan/QT_iCalendar/stargazers
[issues-shield]: https://img.shields.io/github/issues/MartenAlan/QT_iCalendar?style=for-the-badge
[issues-url]: https://github.com/MartenAlan/QT_iCalendar/issues
[license-shield]: https://img.shields.io/github/license/MartenAlan/QT_iCalendar.svg?style=for-the-badge
[license-url]: https://github.com/MartenAlan/QT_iCalendar/blob/main/LICENSE
