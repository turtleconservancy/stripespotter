.PHONY: mac_zip mac_app tester

OS_NAME:=$(shell uname)
VPATH:=core db exif gui utils
BUILDDIR:=build

# debugging build. The thing is Magick++-config produces the -O2 flag
#CXXFLAGS:=-g $(addprefix -I,$(VPATH)) -pipe -D_THREAD_SAFE -pthread -I/usr/lib/wx/include/mac-unicode-debug-2.8 -I/usr/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXDEBUG__ -D__WXMAC__
#LDFLAGS:=-g -L/opt/local/lib -lMagick++ -lMagickWand -lMagickCore -llcms -ltiff -lfreetype -ljpeg -L/opt/local/lib -lfontconfig -lXext -lSM -lICE -lX11 -lXt -llzma -lbz2 -lz -lm -lgomp -lclparser -Wl,-framework,OpenCL -L/System/Library/Frameworks/OpenCL.framework/Versions/A/Libraries -lm -lpthread -lltdl -framework IOKit -framework Carbon -framework Cocoa -framework System -framework QuickTime -framework OpenGL -framework AGL  -lwx_macud-2.8 

# production build
CXXFLAGS:=-O3 $(addprefix -I,$(VPATH)) $(shell Magick++-config --cxxflags; wx-config --cxxflags)
LDFLAGS:=-O3 $(shell Magick++-config --libs; wx-config --libs)

ifeq ($(OS_NAME),Darwin)
	CXXFLAGS:=$(CXXFLAGS) -arch i386
	LDFLAGS:=$(LDFLAGS) -arch i386
endif

APP:=StripeSpotter
VERSION:=2.0
app_obj = FloatImage.o MultiScaleHistogram.o StripeCode.o wxImageUtils.o \
	db-csv.o exif.o AddPicturesDialog.o GUIFrame.o ImageFrame.o \
	SavePictureDialog.o StripeSpotterApp.o StripeSpotterMain.o
compiler_obj = compiler.o StripeCode.o wxImageUtils.o mt19937ar.o \
         FloatImage.o utils.o MultiScaleHistogram.o
benchmarker_obj = benchmarker.o StripeCode.o wxImageUtils.o mt19937ar.o \
	 FloatImage.o utils.o MultiScaleHistogram.o

allpairs_distance_obj = allpairs_distance.o StripeCode.o wxImageUtils.o mt19937ar.o \
	 FloatImage.o utils.o MultiScaleHistogram.o

# first rule = default rule
all: $(BUILDDIR)/$(APP) tester

tester: $(BUILDDIR)/benchmarker $(BUILDDIR)/allpairs_distance $(BUILDDIR)/compiler

mac_zip: $(APP)-v$(VERSION)-mac.zip
mac_app: $(APP).app

$(APP)-v%.zip: $(BUILDDIR)/$(APP) $(APP).app Info.plist AppIcon.icns
	zip -r $@ $(APP).app

$(APP).app: $(BUILDDIR)/$(APP) Info.plist AppIcon.icns
	mkdir -p $@/Contents/MacOS
	mkdir -p $@/Contents/Resources
	cp gui/icon-zebra.bmp $@/Contents/Resources
	cp Info.plist $@/Contents
	cp $(BUILDDIR)/$(APP) $@/Contents/MacOS/$(APP)
	cp AppIcon.icns $@/Contents/Resources

$(BUILDDIR)/$(APP): $(addprefix $(BUILDDIR)/,$(app_obj))
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/benchmarker: $(addprefix $(BUILDDIR)/,$(benchmarker_obj))
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/allpairs_distance: $(addprefix $(BUILDDIR)/,$(allpairs_distance_obj))
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/compiler: $(addprefix $(BUILDDIR)/,$(compiler_obj))
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/%.o: %.cpp | $(BUILDDIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILDDIR):
	mkdir -p $@

clean:
	rm -rf $(APP).app $(BUILDDIR)

