import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:animate_do/animate_do.dart';
import 'dart:math';
import 'dart:async';

void main() {
  runApp(const MLEngineeringApp());
}

class MLEngineeringApp extends StatelessWidget {
  const MLEngineeringApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'ML Engineering',
      theme: ThemeData(
        useMaterial3: true,
        brightness: Brightness.dark,
      ),
      home: const LandingPage(),
      debugShowCheckedModeBanner: false,
    );
  }
}

class LandingPage extends StatefulWidget {
  const LandingPage({Key? key}) : super(key: key);

  @override
  State<LandingPage> createState() => _LandingPageState();
}

class _LandingPageState extends State<LandingPage>
    with TickerProviderStateMixin {
  late AnimationController _textController;
  late AnimationController _shapeController;
  int _currentTextIndex = 0;
  
  final List<String> mlTexts = [
    'ML ENGINEERING',
    'MACHINE LEARNING',
    'DEEP LEARNING',
    'NEURAL NETWORKS',
    'DATA SCIENCE',
    'AI SOLUTIONS',
  ];

  final List<String> shapeEmojis = ['🤖', '⚙️', '🧠', '💡', '🔬', '📊'];

  @override
  void initState() {
    super.initState();
    _textController = AnimationController(
      duration: const Duration(seconds: 4),
      vsync: this,
    )..repeat();

    _shapeController = AnimationController(
      duration: const Duration(seconds: 8),
      vsync: this,
    )..repeat();

    // Change text every 4 seconds
    Timer.periodic(const Duration(seconds: 4), (timer) {
      setState(() {
        _currentTextIndex = (_currentTextIndex + 1) % mlTexts.length;
      });
    });
  }

  @override
  void dispose() {
    _textController.dispose();
    _shapeController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          // Animated gradient background
          _buildAnimatedBackground(),
          
          // Floating background blobs
          _buildBackgroundBlobs(),
          
          // Main content
          Center(
            child: SingleChildScrollView(
              child: Padding(
                padding: const EdgeInsets.all(20.0),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    // Glassmorphism card
                    _buildGlassCard(),
                    
                    const SizedBox(height: 80),
                    
                    // Morphing shapes
                    _buildMorphingShapes(),
                  ],
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildAnimatedBackground() {
    return Container(
      decoration: BoxDecoration(
        gradient: LinearGradient(
          begin: Alignment.topLeft,
          end: Alignment.bottomRight,
          colors: [
            const Color(0xFFee7752).withOpacity(0.8),
            const Color(0xFFe73c7e).withOpacity(0.8),
            const Color(0xFF23a6d5).withOpacity(0.8),
            const Color(0xFF23d5ab).withOpacity(0.8),
          ],
        ),
      ),
      child: AnimatedBuilder(
        animation: _textController,
        builder: (context, child) {
          return Transform.rotate(
            angle: _textController.value * pi / 4,
            child: Container(),
          );
        },
      ),
    );
  }

  Widget _buildBackgroundBlobs() {
    return Stack(
      children: [
        Positioned(
          top: -50,
          left: -50,
          child: FadeInDown(
            duration: const Duration(milliseconds: 1500),
            child: _buildBlob(300, 300, 0.15),
          ),
        ),
        Positioned(
          bottom: -100,
          right: -100,
          child: FadeInUp(
            duration: const Duration(milliseconds: 1800),
            child: _buildBlob(400, 400, 0.15),
          ),
        ),
        Positioned(
          top: MediaQuery.of(context).size.height * 0.3,
          right: MediaQuery.of(context).size.width * 0.1,
          child: FadeInLeft(
            duration: const Duration(milliseconds: 2000),
            child: _buildBlob(250, 250, 0.15),
          ),
        ),
      ],
    );
  }

  Widget _buildBlob(double width, double height, double opacity) {
    return AnimatedBuilder(
      animation: _shapeController,
      builder: (context, child) {
        return Transform.translate(
          offset: Offset(
            sin(_shapeController.value * 2 * pi) * 20,
            cos(_shapeController.value * 2 * pi) * 20,
          ),
          child: Container(
            width: width,
            height: height,
            decoration: BoxDecoration(
              color: Colors.white.withOpacity(opacity),
              borderRadius: BorderRadius.circular(width * 0.3),
            ),
            child: BackdropFilter(
              filter: ImageFilter.blur(sigmaX: 10, sigmaY: 10),
              child: Container(),
            ),
          ),
        );
      },
    );
  }

  Widget _buildGlassCard() {
    return FadeInUp(
      duration: const Duration(milliseconds: 1200),
      child: ClipRRect(
        borderRadius: BorderRadius.circular(40),
        child: BackdropFilter(
          filter: ImageFilter.blur(sigmaX: 10, sigmaY: 10),
          child: Container(
            padding: const EdgeInsets.symmetric(horizontal: 50, vertical: 60),
            decoration: BoxDecoration(
              border: Border.all(
                color: Colors.white.withOpacity(0.3),
                width: 1.5,
              ),
              borderRadius: BorderRadius.circular(40),
              gradient: LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Colors.white.withOpacity(0.15),
                  Colors.white.withOpacity(0.05),
                ],
              ),
              boxShadow: [
                BoxShadow(
                  color: Colors.black.withOpacity(0.3),
                  blurRadius: 60,
                  spreadRadius: 20,
                ),
              ],
            ),
            child: Column(
              mainAxisSize: MainAxisSize.min,
              children: [
                // Main heading with morphing text
                _buildMorphingText(),
                
                const SizedBox(height: 30),
                
                // Subtitle
                FadeInUp(
                  delay: const Duration(milliseconds: 300),
                  duration: const Duration(milliseconds: 1500),
                  child: Text(
                    'Advanced Machine Learning Solutions',
                    textAlign: TextAlign.center,
                    style: GoogleFonts.poppins(
                      fontSize: 18,
                      fontWeight: FontWeight.w300,
                      letterSpacing: 2,
                      color: Colors.white.withOpacity(0.9),
                    ),
                  ),
                ),
                
                const SizedBox(height: 50),
                
                // Animated tech icons
                _buildAnimatedIcons(),
                
                const SizedBox(height: 40),
                
                // CTA Buttons
                _buildCTAButtons(),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildMorphingText() {
    return AnimatedBuilder(
      animation: _textController,
      builder: (context, child) {
        return Transform.scale(
          scale: 1 + (sin(_textController.value * 2 * pi) * 0.05),
          child: ShaderMask(
            shaderCallback: (bounds) {
              return LinearGradient(
                colors: [
                  const Color(0xFF667eea),
                  const Color(0xFF764ba2),
                  const Color(0xFFf093fb),
                  const Color(0xFF4facfe),
                  const Color(0xFF00f2fe),
                ],
                stops: const [0, 0.25, 0.5, 0.75, 1],
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
              ).createShader(bounds);
            },
            child: Text(
              mlTexts[_currentTextIndex],
              textAlign: TextAlign.center,
              style: GoogleFonts.poppins(
                fontSize: 72,
                fontWeight: FontWeight.w900,
                height: 1.1,
                letterSpacing: -2,
                color: Colors.white,
                fontStyle: FontStyle.italic,
              ),
            ),
          ),
        );
      },
    );
  }

  Widget _buildAnimatedIcons() {
    return FadeInUp(
      delay: const Duration(milliseconds: 600),
      duration: const Duration(milliseconds: 1800),
      child: Padding(
        padding: const EdgeInsets.symmetric(vertical: 20),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.center,
          mainAxisSize: MainAxisSize.min,
          children: [
            _buildIconContainer(Icons.radar_chart, 0),
            const SizedBox(width: 30),
            _buildIconContainer(Icons.hub, 1),
            const SizedBox(width: 30),
            _buildIconContainer(Icons.layers, 2),
          ],
        ),
      ),
    );
  }

  Widget _buildIconContainer(IconData icon, int index) {
    return AnimatedBuilder(
      animation: _shapeController,
      builder: (context, child) {
        final rotation = _shapeController.value * 2 * pi * (index % 2 == 0 ? 1 : -1);
        return Transform.translate(
          offset: Offset(
            0,
            sin((rotation + index) * pi) * 8,
          ),
          child: MouseRegion(
            onEnter: (_) => setState(() {}),
            child: Transform.scale(
              scale: 1 + (sin(rotation + index) * 0.05),
              child: Container(
                width: 80,
                height: 80,
                decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(30),
                  border: Border.all(
                    color: Colors.white.withOpacity(0.4),
                    width: 1,
                  ),
                  gradient: LinearGradient(
                    begin: Alignment.topLeft,
                    end: Alignment.bottomRight,
                    colors: [
                      Colors.white.withOpacity(0.2),
                      Colors.white.withOpacity(0.1),
                    ],
                  ),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.white.withOpacity(0.1),
                      blurRadius: 20,
                      spreadRadius: 10,
                    ),
                  ],
                ),
                child: Icon(
                  icon,
                  color: Colors.white.withOpacity(0.8),
                  size: 40,
                ),
              ),
            ),
          ),
        );
      },
    );
  }

  Widget _buildCTAButtons() {
    return FadeInUp(
      delay: const Duration(milliseconds: 900),
      duration: const Duration(milliseconds: 2000),
      child: Wrap(
        spacing: 15,
        runSpacing: 15,
        alignment: WrapAlignment.center,
        children: [
          // Primary button
          Container(
            decoration: BoxDecoration(
              borderRadius: BorderRadius.circular(25),
              gradient: const LinearGradient(
                colors: [Color(0xFF667eea), Color(0xFF764ba2)],
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
              ),
              boxShadow: [
                BoxShadow(
                  color: const Color(0xFF667eea).withOpacity(0.4),
                  blurRadius: 30,
                  spreadRadius: 0,
                ),
              ],
            ),
            child: Material(
              color: Colors.transparent,
              child: InkWell(
                borderRadius: BorderRadius.circular(25),
                onTap: () {
                  ScaffoldMessenger.of(context).showSnackBar(
                    const SnackBar(
                      content: Text('Welcome to ML Engineering!'),
                    ),
                  );
                },
                child: Padding(
                  padding: const EdgeInsets.symmetric(
                    horizontal: 45,
                    vertical: 18,
                  ),
                  child: Text(
                    'EXPLORE PROJECTS',
                    style: GoogleFonts.poppins(
                      fontSize: 14,
                      fontWeight: FontWeight.bold,
                      letterSpacing: 1,
                      color: Colors.white,
                    ),
                  ),
                ),
              ),
            ),
          ),
          
          // Secondary button
          Container(
            decoration: BoxDecoration(
              borderRadius: BorderRadius.circular(25),
              border: Border.all(
                color: Colors.white.withOpacity(0.5),
                width: 2,
              ),
              gradient: LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Colors.white.withOpacity(0.2),
                  Colors.white.withOpacity(0.05),
                ],
              ),
              boxShadow: [
                BoxShadow(
                  color: Colors.white.withOpacity(0.1),
                  blurRadius: 20,
                  spreadRadius: 0,
                ),
              ],
            ),
            child: Material(
              color: Colors.transparent,
              child: InkWell(
                borderRadius: BorderRadius.circular(25),
                onTap: () {
                  ScaffoldMessenger.of(context).showSnackBar(
                    const SnackBar(
                      content: Text('Documentation coming soon!'),
                    ),
                  );
                },
                child: Padding(
                  padding: const EdgeInsets.symmetric(
                    horizontal: 45,
                    vertical: 18,
                  ),
                  child: Text(
                    'DOCUMENTATION',
                    style: GoogleFonts.poppins(
                      fontSize: 14,
                      fontWeight: FontWeight.bold,
                      letterSpacing: 1,
                      color: Colors.white,
                    ),
                  ),
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildMorphingShapes() {
    return FadeInUp(
      delay: const Duration(milliseconds: 1200),
      duration: const Duration(milliseconds: 2000),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: List.generate(
          3,
          (index) => Padding(
            padding: const EdgeInsets.symmetric(horizontal: 20),
            child: _buildMorphingShape(index),
          ),
        ),
      ),
    );
  }

  Widget _buildMorphingShape(int index) {
    return AnimatedBuilder(
      animation: _shapeController,
      builder: (context, child) {
        final rotation = _shapeController.value * 2 * pi;
        final morphValue = sin(rotation + (index * pi / 1.5));
        final borderRadius = 50.0 + (morphValue * 20);
        
        return Transform.rotate(
          angle: index % 2 == 0 ? rotation : -rotation,
          child: Container(
            width: 100,
            height: 100,
            decoration: BoxDecoration(
              borderRadius: BorderRadius.circular(borderRadius),
              border: Border.all(
                color: Colors.white.withOpacity(0.3),
                width: 2,
              ),
              gradient: LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Colors.white.withOpacity(0.1),
                  Colors.white.withOpacity(0.05),
                ],
              ),
              boxShadow: [
                BoxShadow(
                  color: Colors.white.withOpacity(0.1),
                  blurRadius: 20,
                  spreadRadius: 5,
                ),
              ],
            ),
            child: Center(
              child: Text(
                shapeEmojis[index],
                style: const TextStyle(fontSize: 48),
              ),
            ),
          ),
        );
      },
    );
  }
}

// Add this import at the top
import 'dart:ui' as ui;
import 'dart:ui';
