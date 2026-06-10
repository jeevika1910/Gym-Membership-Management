// Mood to OSM amenity mapping
const moodMapping = {
  work: "library",
  date: "restaurant",
  quick: "fast_food",
  budget: "restaurant"
};

// Get user GPS location
function getUserLocation(callback) {
  if (!navigator.geolocation) {
    alert("Geolocation not supported");
    return;
  }

  navigator.geolocation.getCurrentPosition(
    pos => callback({
      lat: pos.coords.latitude,
      lng: pos.coords.longitude
    }),
    () => alert("Please allow location access")
  );
}

// Show nearby places
function showPlaces(mood) {
  document.getElementById("moodScreen").style.display = "none";
  document.getElementById("resultScreen").style.display = "block";
  document.getElementById("resultTitle").innerText = "Nearby places for you 📍";

  const results = document.getElementById("results");
  results.innerHTML = "<p>Loading nearby places… ⏳</p>";

  getUserLocation(userLoc => {
    const type = moodMapping[mood];

    const query = `
      [out:json];
      (
        node(around:2000, ${userLoc.lat}, ${userLoc.lng})[amenity=${type}];
      );
      out;
    `;

    fetch("https://overpass-api.de/api/interpreter", {
      method: "POST",
      body: query
    })
    .then(res => res.json())
    .then(data => {
      results.innerHTML = "";

      if (!data.elements.length) {
        results.innerHTML = "<p>No nearby places found </p>";
        return;
      }

      data.elements.slice(0, 5).forEach(place => {
        results.innerHTML += `
          <div class="card">
            <h3>${place.tags.name || "Unnamed place"}</h3>
            <p>📍 ${place.tags.amenity}</p>
            <a target="_blank"
              href="https://www.openstreetmap.org/?mlat=${place.lat}&mlon=${place.lon}">
              View on Map
            </a>
          </div>
        `;
      });
    })
    .catch(() => {
      results.innerHTML = "<p>Error loading places </p>";
    });
  });
}

// Back button
function goBack() {
  document.getElementById("resultScreen").style.display = "none";
  document.getElementById("moodScreen").style.display = "block";
}