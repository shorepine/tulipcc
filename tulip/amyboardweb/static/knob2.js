const KnobHelper = {};
(function(undefined) {

  const members = {

    /*
    ---------------------------------------------------------------------------
      CSS Knob Creation and Drawing Functions
    ---------------------------------------------------------------------------
    */

    createKnobCSS: function(inputEl, containerClass) {
      const knob = new Knob(inputEl,
          function(knob, indicator) {
            KnobHelper.drawKnobCSS(knob, indicator);
          }),
          $input     = $(knob.element),
          $container = $(`<div class="ui-knob-container ${containerClass}">`),
          $body      = $('<div class="ui-knob ui-knob-shadow">'),
          $indicator = $('<div class="ui-knob-indicator">');

      $container.append($body);
      $container.append($indicator);

      $input.hide();
      $container.insertBefore($input);
      $container.append($input);

      // center knob in container
      $body.css({
        "margin-top": -($body.outerHeight()/2),
        "margin-left":-($body.outerWidth()/2)
      });

      setupKnob(knob, $container[0]);

      return knob;

    },

    drawKnobCSS: function(knob, indicator) {
      const $indicator = $(knob.element).siblings('.ui-knob-indicator');
      $indicator.css({
        left: indicator.x - $indicator.outerWidth()/2,
        top:  indicator.y - $indicator.outerHeight()/2
      });

      const rotateText = `rotate(${(-indicator.angle)}deg)`;
      $indicator.css({
        'transform': rotateText,
        '-webkit-transform': rotateText,
        '-moz-transform': rotateText,
        '-o-transform': rotateText
      });
    },

    /*
    ---------------------------------------------------------------------------
      Canvas Knob Creation and Drawing Functions
    ---------------------------------------------------------------------------
    */

    fillCircleCanvas: function(context, x, y, radius) {
      context.beginPath();
      context.arc(x, y, radius, 0, Math.PI*2, true);
      context.closePath();
      context.fill();
    },

    drawPositionIndicatorCanvas: function(context, knob, indicator) {
      const indicatorRadius = 5;

      context.fillStyle = "#efefef";
      KnobHelper.fillCircleCanvas(context, 0, 0, indicatorRadius);
    },

    drawRotateIndicatorCanvas: function(context, knob, indicator) {
      const knobRadius = 32,
        indicatorW = knobRadius*2,
        indicatorH = 10;

      context.fillStyle = "#efefef";
      context.fillRect(-indicatorW/2, -indicatorH/2, indicatorW, indicatorH);
    },

    drawPositionRotateIndicatorCanvas: function(context, knob, indicator) {
      const knobRadius = 32,
        indicatorW = 10,
        indicatorH = 6;

      context.fillStyle = "#efefef";
      context.fillRect(-indicatorW/2, -indicatorH/2, indicatorW, indicatorH);
    },

    drawIndicatorCanvas: function(context, knob, indicator) {
      context.save();

      context.translate(indicator.x, indicator.y);
      context.rotate(-indicator.angle*(Math.PI/180));

      if(knob.options.indicatorAutoPosition && knob.options.indicatorAutoRotate) {
        KnobHelper.drawPositionRotateIndicatorCanvas(context, knob, indicator);
      }
      else if(knob.options.indicatorAutoPosition) {
        KnobHelper.drawPositionIndicatorCanvas(context, knob, indicator);
      }
      else if(knob.options.indicatorAutoRotate) {
        KnobHelper.drawRotateIndicatorCanvas(context, knob, indicator);
      }

      context.restore();
    },

    drawKnobBodyCanvas: function(context, knob) {
      const knobRadius = 32;

      context.clearRect(0, 0, context.canvas.width, context.canvas.height);

      //draw the knob background
      context.fillStyle = "#008";
      KnobHelper.fillCircleCanvas(context, context.canvas.width/2, context.canvas.height/2, knobRadius);
    },

    drawKnobCanvas: function(knob, indicator) {
      const canvas = $(knob.element).siblings('canvas')[0];

      if(!canvas) { return; }

      const context = canvas.getContext('2d');

      KnobHelper.drawKnobBodyCanvas(context, knob);

      KnobHelper.drawIndicatorCanvas(context, knob, indicator);
    },

    createKnobCanvas: function(inputEl, containerClass) {
      const knob = new Knob(inputEl,
          function(knob, indicator) {
            KnobHelper.drawKnobCanvas(knob, indicator);
          }),
          $input     = $(knob.element),
          $container = $(`<div class="ui-knob-container ${containerClass}">`),
          $body      = $(`<canvas class="${containerClass}">`);

      $container.append($body);

      $input.hide();
      $container.insertBefore($input);
      $container.append($input);

      $body[0].width  = $body[0].parentElement.clientWidth;
      $body[0].height = $body[0].parentElement.clientHeight;

      setupKnob(knob, $container[0]);

      return knob;
    },

    /*
    ---------------------------------------------------------------------------
      SVG Knob Creation and Drawing Functions
    ---------------------------------------------------------------------------
    */

    drawKnobSVG: function(knob, indicator) {
      const $indicator = $($(knob.element).siblings('svg').children()[1]);
      $indicator.attr('transform',
        [
          `translate(${indicator.x},${indicator.y})`,
          `rotate(${-indicator.angle})`
        ].join()
      );
    },
  
    createKnobSVG: function(inputEl, id) {
      const knob = new Knob(inputEl,
          function(knob, indicator) {
            KnobHelper.drawKnobSVG(knob, indicator);
          }),
          $input     = $(knob.element),
          $container = $(`<div class="ui-knob-container ${id}">`),
          $svg       = $(`<svg class="${id}" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">`);

      $container.append($svg);
  
      $input.hide();
      $container.insertBefore($input);
      $container.append($input);
  
      // Must define SVG elements using NS
      const body = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
      body.setAttributeNS(null, 'r',  32);
      body.setAttributeNS(null, 'fill', 'red');
      // center knob in container
      body.setAttributeNS(null, 'transform', `translate(${$container.outerWidth()/2}, ${$container.outerHeight()/2})`);
  
      if(knob.options.indicatorAutoPosition && knob.options.indicatorAutoRotate) {
        const [w,h] = [10,6]
        $indicator = document.createElementNS('http://www.w3.org/2000/svg', 'rect');
        $indicator.setAttributeNS(null, 'class', 'ui-indicator position-rotate-svg-knob');
        $indicator.setAttributeNS(null, 'fill', 'white');
        $indicator.setAttributeNS(null, 'width',  w);
        $indicator.setAttributeNS(null, 'height', h);
        $indicator.setAttributeNS(null, 'x', -w/2);
        $indicator.setAttributeNS(null, 'y', -h/2);
      }
      else if(knob.options.indicatorAutoPosition) {
        $indicator = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
        $indicator.setAttributeNS(null, 'class', 'ui-indicator position-svg-knob');
        $indicator.setAttributeNS(null, 'r',  5);
        $indicator.setAttributeNS(null, 'fill', 'white');
      }
      else if(knob.options.indicatorAutoRotate) {
        const [w,h] = [64,10]
        $indicator = document.createElementNS('http://www.w3.org/2000/svg', 'rect');
        $indicator.setAttributeNS(null, 'class', 'ui-indicator rotate-svg-knob');
        $indicator.setAttributeNS(null, 'fill', 'white');
        $indicator.setAttributeNS(null, 'width',  w);
        $indicator.setAttributeNS(null, 'height', h);
        $indicator.setAttributeNS(null, 'x', -w/2);
        $indicator.setAttributeNS(null, 'y', -h/2);
      }

      $svg.append(body);
      $svg.append($indicator);

      setupKnob(knob, $container[0]);

      return knob;
    },
  
  
    /*
    ---------------------------------------------------------------------------
      Sprite Knob Creation and Drawing Functions
    ---------------------------------------------------------------------------
    */

    drawCenteredImageSprite: function(context, image) {
      if(context && image) {
        context.drawImage(image, -image.width/2, -image.height/2, image.width, image.height);
      }
    },

    drawIndicatorImageSprite: function(image) {
      return function(c, k, i) { KnobHelper.drawCenteredImageSprite(c, image); }
    },

    drawIndicatorSprite: function(context, knob, indicator, drawIndicatorFn) {
      context.save();

      context.translate(indicator.x, indicator.y);
      context.rotate(-indicator.angle*(Math.PI/180));

      if(drawIndicatorFn)
        drawIndicatorFn(context, knob, indicator);

      context.restore();
    },

    drawKnobSprite: function(knob, indicator, drawIndicatorFn, spriteOffset, spriteImage) {
      const canvas = $(knob.element).siblings('canvas')[0];

      if(!canvas) { return; }

      const context = canvas.getContext('2d'),
          spriteX = Math.floor(context.canvas.width/2  - knob.options.spriteWidth/2),
          spriteY = Math.floor(context.canvas.height/2 - knob.options.spriteHeight/2);

      context.clearRect(0, 0, context.canvas.width, context.canvas.height);

      context.drawImage(spriteImage,
                spriteOffset.x, spriteOffset.y, knob.options.spriteWidth, knob.options.spriteHeight,
                spriteX,        spriteY,        knob.options.spriteWidth, knob.options.spriteHeight);

      KnobHelper.drawIndicatorSprite(context, knob, indicator, drawIndicatorFn);
    },

    loadImage: function(callback, path) {
      const image = new Image();
      image.onload = function() {
        if(callback) { callback.call(this); }
      }
      image.src = path;
      return image;
    },

    createKnobSprite: function(inputEl, containerClass, spriteImagePath, indicatorImagePath) {
      let spriteImage, indicatorImage
      const knob = new Knob(inputEl,
            function(knob, indicator, spriteOffset) {
              KnobHelper.drawKnobSprite(knob, indicator, KnobHelper.drawIndicatorImageSprite(indicatorImage), spriteOffset, spriteImage);
            }
          ),
          $input     = $(knob.element),
          $container = $(`<div class="ui-knob-container ${containerClass}">`),
          $body      = $('<canvas>');

      $container.append($body);

      $input.hide();
      $container.insertBefore($input);
      $container.append($input);

      const context = $body[0].getContext('2d');

      $body[0].width  = $body[0].parentElement.clientWidth;
      $body[0].height = $body[0].parentElement.clientHeight;

      if(typeof indicatorImagePath !== "undefined") {
        indicatorImage = KnobHelper.loadImage(function() {
          spriteImage = KnobHelper.loadImage(function() {
            setupKnob(knob, $container[0]);
          }, spriteImagePath);
        }, indicatorImagePath);
      }
      else {
        spriteImage = KnobHelper.loadImage(function() {
          setupKnob(knob, $container[0]);
        }, spriteImagePath);
      }

      return knob;
    },

  } // end members

  for (const key in members) {
    KnobHelper[key] = members[key];
  }

})();



const activeKnobs = {};

function getPlatform() {
  // 2022 way of detecting. Note : this userAgentData feature is available only in secure contexts (HTTPS)
  if (typeof navigator.userAgentData !== 'undefined' && navigator.userAgentData != null) {
    return navigator.userAgentData.platform;
  }
  // Deprecated but still works for most of the browser
  if (typeof navigator.platform !== 'undefined') {
    if (typeof navigator.userAgent !== 'undefined' && /android/.test(navigator.userAgent.toLowerCase())) {
      // android device’s navigator.platform is often set as 'linux', so let’s use userAgent for them
      return 'android';
    }
    return navigator.platform;
  }
  return 'unknown';
}

function setupKnob(knob, container) {

  // var rect = container.getBoundingClientRect();
  // knob.setPosition(rect.left + container.clientLeft, rect.top + container.clientTop);
  knob.setPosition(container.offsetLeft, container.offsetTop);
  knob.setDimensions(container.clientWidth, container.clientHeight);

  // Detect touch capable client
  if ('ontouchstart' in window) {

    container.addEventListener('touchstart', function(e) {
      // reset the position in case knob moved
      knob.setPosition(container.offsetLeft, container.offsetTop);

      const timeStamp = e.timeStamp.getTime ? e.timeStamp.getTime() : e.timeStamp;
      // Keep track of the knobs currently being touched to support multitouch.
      activeKnobs[e.targetTouches[0].identifier] = knob;
      knob.doTouchStart(e.targetTouches, timeStamp);
      e.preventDefault();
    }, false);

    document.addEventListener('touchmove', function(e) {
      const timeStamp = e.timeStamp.getTime ? e.timeStamp.getTime() : e.timeStamp;
      // Support multi-touch knobs by only passing the appropriate touch events.
      for(const i = 0, l = e.changedTouches.length; i < l; i++) {
        const k = activeKnobs[e.changedTouches[i].identifier];
        if(typeof k !== "undefined") {
          k.doTouchMove([e.changedTouches[i]], timeStamp, e.scale);
        }
      }

    }, false);

    document.addEventListener('touchend', function(e) {
      const timeStamp = e.timeStamp.getTime ? e.timeStamp.getTime() : e.timeStamp;
      knob.doTouchEnd(timeStamp);
    }, false);

    document.addEventListener('touchcancel', function(e) {
      const timeStamp = e.timeStamp.getTime ? e.timeStamp.getTime() : e.timeStamp;
      knob.doTouchEnd(timeStamp);
    }, false);

  } else {
    // No touch capable client detected, use mouse interactions
    let mousedown = false;
    const naturalScrolling = !/win|linux/.test(getPlatform().toLowerCase())

    container.addEventListener('mousedown', function(e) {
      // reset the position in case knob moved
      knob.setPosition(container.offsetLeft, container.offsetTop);

      knob.doTouchStart([{
        pageX: e.pageX,
        pageY: e.pageY
      }], e.timeStamp);

      mousedown = true;
    }, false);

    document.addEventListener('mousemove', function(e) {
      if (!mousedown) { return; }

      knob.doTouchMove([{
        pageX: e.pageX,
        pageY: e.pageY
      }], e.timeStamp);

      mousedown = true;

      // Prevent selection on drag
      if (e.preventDefault)
        e.preventDefault();
      e.returnValue = false;
    }, false);

    document.addEventListener('mouseup', function(e) {
      if (!mousedown) { return; }

      knob.doTouchEnd(e.timeStamp);

      mousedown = false;
    }, false);

    // Handle scroll for webkit
    container.addEventListener('mousewheel', function(e) {
      // reset the position in case knob moved
      knob.setPosition(container.offsetLeft, container.offsetTop);

      const delta = naturalScrolling ? e.wheelDelta : -e.wheelDelta
      knob.doMouseScroll(delta, e.timeStamp, e.pageX, e.pageY);
      // Prevent page scroll
      if (e.preventDefault)
        e.preventDefault();
      e.returnValue = false;
    }, false);

    // Handle scroll for gecko
    // container.addEventListener('MozMousePixelScroll', function(e) {
    container.addEventListener('DOMMouseScroll', function(e) {
      // reset the position in case knob moved
      knob.setPosition(container.offsetLeft, container.offsetTop);
      let delta = -4*e.detail
      delta = naturalScrolling ? -delta : -delta
      knob.doMouseScroll(delta, e.timeStamp, e.pageX, e.pageY);
      // Prevent page scroll
      if (e.preventDefault)
        e.preventDefault();
      e.returnValue = false;
    }, false);
  }
}