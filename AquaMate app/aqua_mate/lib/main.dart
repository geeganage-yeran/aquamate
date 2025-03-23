//import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:percent_indicator/percent_indicator.dart';
import 'package:firebase_core/firebase_core.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  if (kIsWeb) {
    await Firebase.initializeApp(
      options: FirebaseOptions(
        apiKey: "AIzaSyCDk2zkSxkgR29GYQfVcoRjZvvqsldadFw",
        appId: "1:1004420116912:web:915ba3d752e3bc309de675",
        messagingSenderId: "1004420116912",
        projectId: "aquamate-4ad2a",
        databaseURL: "https://aquamate-4ad2a-default-rtdb.firebaseio.com/",
      ),
    );
  } else {
    await Firebase.initializeApp(); // Initialize Firebase for other platforms
  }
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  bool feedFishState = false; // State for Feed Fish switch
  bool lightState = false; // State for Aquarium Light switch
  double tds = 0;
  double temperature = 0;
  double turbidity = 0;

  @override
  void initState() {
    super.initState();
    fetchData();
  }

  void fetchData() {
    DatabaseReference ref = FirebaseDatabase.instance.ref("aquarium/status");
    ref.onValue.listen((DatabaseEvent event) {
      final data = event.snapshot.value as Map<dynamic, dynamic>?;
      if (data != null) {
        setState(() {
          tds = data['tds']?.toDouble() ?? 0.0;
          temperature = data['temperature']?.toDouble() ?? 0.0;
          turbidity = data['turbidity']?.toDouble() ?? 0.0;
          feedFishState = data['feedStatus'] == 1;
          lightState = data['lightStatus'] == 1;
        });
      }
    });
  }

  void updateFeedStatus(bool status) {
    DatabaseReference ref = FirebaseDatabase.instance.ref("aquarium/status");
    ref.update({
      'feedStatus': status ? 1 : 0,
    });
  }

  void updateLightStatus(bool status) {
    DatabaseReference ref = FirebaseDatabase.instance.ref("aquarium/status");
    ref.update({
      'lightStatus': status ? 1 : 0,
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        textTheme: GoogleFonts.poppinsTextTheme(),
      ),
      home: Scaffold(
        appBar: AppBar(
          backgroundColor: Colors.white,
          elevation: 0,
          toolbarHeight: 120,
          title: Center(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                Text(
                  'AQUAMATE',
                  style: TextStyle(
                    color: Color(0xFF070417),
                    fontWeight: FontWeight.w900,
                    fontSize: 36,
                  ),
                ),
                SizedBox(height: 5),
                Text(
                  'Smarter Aquariums, Happier Fish',
                  style: TextStyle(
                    color: Color(0xFF070417),
                    fontSize: 18,
                  ),
                ),
              ],
            ),
          ),
        ),
        body: SingleChildScrollView(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              SizedBox(height: 30),
              buildCircularPercentIndicator(
                'TDS Level (Water Purity)',
                tds,
                'Shows the amount of dissolved solids. High levels may indicate impurities.',
                isTemperature: false,
              ),
              SizedBox(height: 40),
              buildCircularPercentIndicator(
                'Turbidity Level (Water Clarity)',
                turbidity,
                'Measures clarity. Higher values mean cloudier water, which may affect fish health.',
                isTemperature: false,
              ),
              SizedBox(height: 40),
              buildCircularPercentIndicator(
                'Temperature Level (Water Warmth)',
                temperature,
                'Displays current temperature as a percentage of the ideal range for fish. Stable warmth is key for tank health.',
                isTemperature: true,
              ),
              SizedBox(height: 50),
              buildToggleSwitch('Feed Fish', feedFishState,
                  "Easily feed your fish with a single tap to ensure they get their meals on time. Keep your aquatic friends healthy and happy by managing their feeding schedule conveniently."),
              SizedBox(height: 20),
              buildToggleSwitch('Aquarium Light', lightState,
                  "Control your tank's lighting to create a balanced environment for your fish. Easily switch the light on or off to mimic natural day and night cycles, promoting a healthy habitat."),
            ],
          ),
        ),
      ),
    );
  }

  Widget buildCircularPercentIndicator(
      String label, double value, String description,
      {bool isTemperature = false}) {
    Color progressColor =
        getIndicatorColor(value, isTemperature: isTemperature);

    // Directly use the value as a percentage
    double percent = value / 100;

    return Row(
      children: [
        CircularPercentIndicator(
          radius: 50.0,
          lineWidth: 10.0,
          percent: percent,
          center: Text(
            isTemperature
                ? '${value.toStringAsFixed(1)}°C'
                : '${(percent * 100).toStringAsFixed(0)}%',
            style: TextStyle(
              color: Color(0xFF070417),
              fontWeight: FontWeight.w900,
              fontSize: 20,
            ),
          ),
          progressColor: progressColor,
        ),
        SizedBox(width: 20),
        Expanded(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                label,
                style: TextStyle(
                  color: Color(0xFF070417),
                  fontWeight: FontWeight.bold,
                  fontSize: 16,
                ),
              ),
              SizedBox(height: 4),
              Text(
                description,
                style: TextStyle(
                  color: Color(0xFF070417),
                  fontSize: 12,
                ),
              ),
            ],
          ),
        ),
      ],
    );
  }

  Color getIndicatorColor(double value, {bool isTemperature = false}) {
    if (isTemperature) {
      return value > 27.0 ? const Color(0xFFC62828) : Colors.green;
    } else {
      if (value > 50) {
        return Colors.red[800]!;
      } else if (value > 25) {
        return Colors.yellow[700]!;
      } else {
        return Colors.green;
      }
    }
  }

  Widget buildToggleSwitch(String label, bool initialState, String subtitle) {
    return Row(
      children: [
        Switch(
          value: initialState,
          onChanged: (bool value) {
            setState(() {
              if (label == 'Feed Fish') {
                feedFishState = value;
                updateFeedStatus(value);
              } else if (label == 'Aquarium Light') {
                lightState = value;
                updateLightStatus(value);
              }
            });
          },
          activeColor: const Color.fromARGB(255, 65, 167, 68),
          inactiveThumbColor: const Color(0xFFC62828),
        ),
        SizedBox(width: 20),
        Expanded(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                label,
                style: TextStyle(
                  color: Color(0xFF070417),
                  fontWeight: FontWeight.bold,
                  fontSize: 16,
                ),
              ),
              Text(
                subtitle,
                style: TextStyle(
                  color: Color(0xFF070417),
                  fontSize: 12,
                ),
              ),
            ],
          ),
        ),
      ],
    );
  }
}
