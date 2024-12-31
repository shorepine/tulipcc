"""Filter design."""
import math
from ulab import numpy
from ulab import numpy as np
from ulab import scipy as spy

from ...numpy import (atleast_1d, poly, asarray, prod, size, append, nonzero, zeros_like, isreal)

def butter(N, Wn, btype='low', analog=False, output='ba', fs=None):
    """
    Butterworth digital and analog filter design.

    Design an Nth-order digital or analog Butterworth filter and return
    the filter coefficients.

    Parameters
    ----------
    N : int
        The order of the filter.
    Wn : array_like
        The critical frequency or frequencies. For lowpass and highpass
        filters, Wn is a scalar; for bandpass and bandstop filters,
        Wn is a length-2 sequence.

        For a Butterworth filter, this is the point at which the gain
        drops to 1/sqrt(2) that of the passband (the "-3 dB point").

        For digital filters, `Wn` are in the same units as `fs`.  By default,
        `fs` is 2 half-cycles/sample, so these are normalized from 0 to 1,
        where 1 is the Nyquist frequency. (`Wn` is thus in
        half-cycles / sample.)

        For analog filters, `Wn` is an angular frequency (e.g. rad/s).
        btype : {'lowpass', 'highpass', 'bandpass', 'bandstop'}, optional
        The type of filter.  Default is 'lowpass'.
        analog : bool, optional
        When True, return an analog filter, otherwise a digital filter is
        returned.
        output : {'ba', 'zpk', 'sos'}, optional
        Type of output:  numerator/denominator ('ba'), pole-zero ('zpk'), or
        second-order sections ('sos'). Default is 'ba' for backwards
        compatibility, but 'sos' should be used for general-purpose filtering.
    fs : float, optional
        The sampling frequency of the digital system.

        .. versionadded:: 1.2.0

    Returns
    -------
    b, a : ndarray, ndarray
        Numerator (`b`) and denominator (`a`) polynomials of the IIR filter.
        Only returned if ``output='ba'``.
    z, p, k : ndarray, ndarray, float
        Zeros, poles, and system gain of the IIR filter transfer
        function.  Only returned if ``output='zpk'``.
    sos : ndarray
        Second-order sections representation of the IIR filter.
        Only returned if ``output=='sos'``.

    See Also
    --------
    buttord, buttap

    Notes
    -----
    The Butterworth filter has maximally flat frequency response in the
    passband.

    The ``'sos'`` output parameter was added in 0.16.0.

    If the transfer function form ``[b, a]`` is requested, numerical
    problems can occur since the conversion between roots and
    the polynomial coefficients is a numerically sensitive operation,
    even for N >= 4. It is recommended to work with the SOS
    representation.

    Examples
    --------
    Design an analog filter and plot its frequency response, showing the
    critical points:

    >>> from scipy import signal
    >>> import matplotlib.pyplot as plt

    >>> b, a = signal.butter(4, 100, 'low', analog=True)
    >>> w, h = signal.freqs(b, a)
    >>> plt.semilogx(w, 20 * np.log10(abs(h)))
    >>> plt.title('Butterworth filter frequency response')
    >>> plt.xlabel('Frequency [radians / second]')
    >>> plt.ylabel('Amplitude [dB]')
    >>> plt.margins(0, 0.1)
    >>> plt.grid(which='both', axis='both')
    >>> plt.axvline(100, color='green') # cutoff frequency
    >>> plt.show()

    Generate a signal made up of 10 Hz and 20 Hz, sampled at 1 kHz

    >>> t = np.linspace(0, 1, 1000, False)  # 1 second
    >>> sig = np.sin(2*np.pi*10*t) + np.sin(2*np.pi*20*t)
    >>> fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True)
    >>> ax1.plot(t, sig)
    >>> ax1.set_title('10 Hz and 20 Hz sinusoids')
    >>> ax1.axis([0, 1, -2, 2])

    Design a digital high-pass filter at 15 Hz to remove the 10 Hz tone, and
    apply it to the signal. (It's recommended to use second-order sections
    format when filtering, to avoid numerical error with transfer function
    (``ba``) format):

    >>> sos = signal.butter(10, 15, 'hp', fs=1000, output='sos')
    >>> filtered = signal.sosfilt(sos, sig)
    >>> ax2.plot(t, filtered)
    >>> ax2.set_title('After 15 Hz high-pass filter')
    >>> ax2.axis([0, 1, -2, 2])
    >>> ax2.set_xlabel('Time [seconds]')
    >>> plt.tight_layout()
    >>> plt.show()
    """
    return iirfilter(N, Wn, btype=btype, analog=analog,
                     output=output, ftype='butter', fs=fs)

def iirfilter(N, Wn, rp=None, rs=None, btype='band', analog=False,
              ftype='butter', output='ba', fs=None):
    """
    IIR digital and analog filter design given order and critical points.

    Design an Nth-order digital or analog filter and return the filter
    coefficients.

    Parameters
    ----------
    N : int
        The order of the filter.
    Wn : array_like
        A scalar or length-2 sequence giving the critical frequencies.

        For digital filters, `Wn` are in the same units as `fs`. By default,
        `fs` is 2 half-cycles/sample, so these are normalized from 0 to 1,
        where 1 is the Nyquist frequency. (`Wn` is thus in
        half-cycles / sample.)

        For analog filters, `Wn` is an angular frequency (e.g., rad/s).
    rp : float, optional
        For Chebyshev and elliptic filters, provides the maximum ripple
        in the passband. (dB)
    rs : float, optional
        For Chebyshev and elliptic filters, provides the minimum attenuation
        in the stop band. (dB)
    btype : {'bandpass', 'lowpass', 'highpass', 'bandstop'}, optional
        The type of filter.  Default is 'bandpass'.
    analog : bool, optional
        When True, return an analog filter, otherwise a digital filter is
        returned.
    ftype : str, optional
        The type of IIR filter to design:

            - Butterworth   : 'butter'
            - Chebyshev I   : 'cheby1'
            - Chebyshev II  : 'cheby2'
            - Cauer/elliptic: 'ellip'
            - Bessel/Thomson: 'bessel'

    output : {'ba', 'zpk', 'sos'}, optional
        Filter form of the output:

            - second-order sections (recommended): 'sos'
            - numerator/denominator (default)    : 'ba'
            - pole-zero                          : 'zpk'

        In general the second-order sections ('sos') form  is
        recommended because inferring the coefficients for the
        numerator/denominator form ('ba') suffers from numerical
        instabilities. For reasons of backward compatibility the default
        form is the numerator/denominator form ('ba'), where the 'b'
        and the 'a' in 'ba' refer to the commonly used names of the
        coefficients used.

        Note: Using the second-order sections form ('sos') is sometimes
        associated with additional computational costs: for
        data-intense use cases it is therefore recommended to also
        investigate the numerator/denominator form ('ba').

    fs : float, optional
        The sampling frequency of the digital system.

        .. versionadded:: 1.2.0

    Returns
    -------
    b, a : ndarray, ndarray
        Numerator (`b`) and denominator (`a`) polynomials of the IIR filter.
        Only returned if ``output='ba'``.
    z, p, k : ndarray, ndarray, float
        Zeros, poles, and system gain of the IIR filter transfer
        function.  Only returned if ``output='zpk'``.
    sos : ndarray
        Second-order sections representation of the IIR filter.
        Only returned if ``output=='sos'``.

    See Also
    --------
    butter : Filter design using order and critical points
    cheby1, cheby2, ellip, bessel
    buttord : Find order and critical points from passband and stopband spec
    cheb1ord, cheb2ord, ellipord
    iirdesign : General filter design using passband and stopband spec

    Notes
    -----
    The ``'sos'`` output parameter was added in 0.16.0.

    Examples
    --------
    Generate a 17th-order Chebyshev II analog bandpass filter from 50 Hz to
    200 Hz and plot the frequency response:

    >>> from scipy import signal
    >>> import matplotlib.pyplot as plt

    >>> b, a = signal.iirfilter(17, [2*np.pi*50, 2*np.pi*200], rs=60,
    ...                         btype='band', analog=True, ftype='cheby2')
    >>> w, h = signal.freqs(b, a, 1000)
    >>> fig = plt.figure()
    >>> ax = fig.add_subplot(1, 1, 1)
    >>> ax.semilogx(w / (2*np.pi), 20 * np.log10(np.maximum(abs(h), 1e-5)))
    >>> ax.set_title('Chebyshev Type II bandpass frequency response')
    >>> ax.set_xlabel('Frequency [Hz]')
    >>> ax.set_ylabel('Amplitude [dB]')
    >>> ax.axis((10, 1000, -100, 10))
    >>> ax.grid(which='both', axis='both')
    >>> plt.show()

    Create a digital filter with the same properties, in a system with
    sampling rate of 2000 Hz, and plot the frequency response. (Second-order
    sections implementation is required to ensure stability of a filter of
    this order):

    >>> sos = signal.iirfilter(17, [50, 200], rs=60, btype='band',
    ...                        analog=False, ftype='cheby2', fs=2000,
    ...                        output='sos')
    >>> w, h = signal.sosfreqz(sos, 2000, fs=2000)
    >>> fig = plt.figure()
    >>> ax = fig.add_subplot(1, 1, 1)
    >>> ax.semilogx(w, 20 * np.log10(np.maximum(abs(h), 1e-5)))
    >>> ax.set_title('Chebyshev Type II bandpass frequency response')
    >>> ax.set_xlabel('Frequency [Hz]')
    >>> ax.set_ylabel('Amplitude [dB]')
    >>> ax.axis((10, 1000, -100, 10))
    >>> ax.grid(which='both', axis='both')
    >>> plt.show()

    """
    ftype, btype, output = [x.lower() for x in (ftype, btype, output)]
    Wn = asarray(Wn)
    if fs is not None:
        if analog:
            raise ValueError("fs cannot be specified for an analog filter")
        Wn = 2*Wn/fs

    try:
        btype = band_dict[btype]
    except KeyError as e:
        raise ValueError("'%s' is an invalid bandtype for filter." % btype) from e

    try:
        typefunc = filter_dict[ftype][0]
    except KeyError as e:
        raise ValueError("'%s' is not a valid basic IIR filter." % ftype) from e

    if output not in ['ba', 'zpk', 'sos']:
        raise ValueError("'%s' is not a valid output form." % output)

    if rp is not None and rp < 0:
        raise ValueError("passband ripple (rp) must be positive")

    if rs is not None and rs < 0:
        raise ValueError("stopband attenuation (rs) must be positive")

    # Get analog lowpass prototype
    if typefunc == buttap:
        z, p, k = typefunc(N)
    else:
        raise NotImplementedError("'%s' not implemented in iirfilter." % ftype)

    # Pre-warp frequencies for digital filter design
    if not analog:
        if numpy.any(Wn <= 0) or numpy.any(Wn >= 1):
            if fs is not None:
                raise ValueError("Digital filter critical frequencies "
                                 "must be 0 < Wn < fs/2 (fs={} -> fs/2={})".format(fs, fs/2))
            raise ValueError("Digital filter critical frequencies "
                             "must be 0 < Wn < 1")
        fs = 2.0
        b = []
        for x in Wn:
            b.append(2 * fs * math.tan(np.pi * x / fs))
        warped = np.array(b)
    else:
        warped = Wn

    # transform to lowpass, bandpass, highpass, or bandstop
    if btype in ('lowpass', 'highpass'):
        if size(Wn) != 1:
            raise ValueError('Must specify a single critical frequency Wn for lowpass or highpass filter')

        if btype == 'lowpass':
            z, p, k = lp2lp_zpk(z, p, k, wo=warped[0])
        elif btype == 'highpass':
            z, p, k = lp2hp_zpk(z, p, k, wo=warped[0])
    elif btype in ('bandpass', 'bandstop'):
        try:
            bw = warped[1] - warped[0]
            wo = math.sqrt(warped[0] * warped[1])
        except IndexError as e:
            raise ValueError('Wn must specify start and stop frequencies for bandpass or bandstop '
                             'filter') from e

        if btype == 'bandpass':
            z, p, k = lp2bp_zpk(z, p, k, wo=wo, bw=bw)
        elif btype == 'bandstop':
            z, p, k = lp2bs_zpk(z, p, k, wo=wo, bw=bw)
    else:
        raise NotImplementedError("'%s' not implemented in iirfilter." % btype)
    # Find discrete equivalent if necessary
    if not analog:
        z, p, k = bilinear_zpk(z, p, k, fs=fs)

    # Transform to proper out type (pole-zero, state-space, numer-denom)
    if output == 'zpk':
        return z, p, k
    elif output == 'ba':
        return zpk2tf(z, p, k)
    elif output == 'sos':
        return zpk2sos(z, p, k)

def zpk2tf(z, p, k):
    """
    Return polynomial transfer function representation from zeros and poles

    Parameters
    ----------
    z : array_like
        Zeros of the transfer function.
    p : array_like
        Poles of the transfer function.
    k : float
        System gain.

    Returns
    -------
    b : ndarray
        Numerator polynomial coefficients.
    a : ndarray
        Denominator polynomial coefficients.

    """
    z = atleast_1d(z)
    k = atleast_1d(k)
    if len(z.shape) > 1:
        temp = poly(z[0])
        b = np.empty((z.shape[0], z.shape[1] + 1), temp.dtype.char)
        if len(k) == 1:
            k = [k[0]] * z.shape[0]
        for i in range(z.shape[0]):
            b[i] = k[i] * poly(z[i])
    else:
        b = k * poly(z)
    a = atleast_1d(poly(p))
    # Use real output if possible. Copied from numpy.np.poly, since
    # we can't depend on a specific version of numpy.
    if b.dtype == np.complex:
        # if complex roots are all complex conjugates, the roots are real.
        roots = asarray(z, complex)
        pos_roots = np.compress(roots.imag > 0, roots)
        neg_roots = np.conjugate(np.compress(roots.imag < 0, roots))
        if len(pos_roots) > 0 and len(pos_roots) == len(neg_roots):
            p = np.sort_complex(neg_roots)
            q = np.sort_complex(pos_roots)
            if np.all(p == q):
                b = b.real.copy()

    if a.dtype == np.complex:
        # if complex roots are all complex conjugates, the roots are real.
        roots = asarray(p, complex)
        pos_roots = np.compress(roots.imag > 0, roots)
        neg_roots = np.conjugate(np.compress(roots.imag < 0, roots))
        if len(pos_roots) > 0 and len(pos_roots) == len(neg_roots):
            p = np.sort_complex(neg_roots)
            q = np.sort_complex(pos_roots)
            if np.all(p == q):
                a = a.real.copy()

    return b, a

def _to_tuple(a):
    result = []
    for x in a:
       result.append([x.real, x.imag])
    return result

def _to_complex(a):
    result = np.array([], dtype=np.complex)
    for x in a:
       t = np.array([complex(x[0] + x[1] * 1j)], dtype=np.complex)
       result = np.concatenate((result, t), axis=0)
    return result

def lexsort(z):
   z = _to_tuple(z)
   return sorted(range(len(z)), key=lambda i: (z[i][0],abs(z[i][1])))

def _lexsort(z):
   z = _to_tuple(z)
   z = sorted(z,key=lambda x:(x[0], abs(x[1])))
   return _to_complex(z)

def _cplxreal(z, tol=None):
    """
    Split into complex and real parts, combining conjugate pairs.
    The 1-D input vector `z` is split up into its complex (`zc`) and real (`zr`)
    elements. Every complex element must be part of a complex-conjugate pair,
    which are combined into a single number (with positive imaginary part) in
    the output. Two complex numbers are considered a conjugate pair if their
    real and imaginary parts differ in magnitude by less than ``tol * abs(z)``.
    Parameters
    ----------
    z : array_like
        Vector of complex numbers to be sorted and split
    tol : float, optional
        Relative tolerance for testing realness and conjugate equality.
        Default is ``100 * spacing(1)`` of `z`'s data type (i.e., 2e-14 for
        float64)
    Returns
    -------
    zc : ndarray
        Complex elements of `z`, with each pair represented by a single value
        having positive imaginary part, sorted first by real part, and then
        by magnitude of imaginary part. The pairs are averaged when combined
        to reduce error.
    zr : ndarray
        Real elements of `z` (those having imaginary part less than
        `tol` times their magnitude), sorted by value.
    Raises
    ------
    ValueError
        If there are any complex numbers in `z` for which a conjugate
        cannot be found.
    See Also
    --------
    _cplxpair
    Examples
    --------
    >>> a = [4, 3, 1, 2-2j, 2+2j, 2-1j, 2+1j, 2-1j, 2+1j, 1+1j, 1-1j]
    >>> zc, zr = _cplxreal(a)
    >>> print(zc)
    [ 1.+1.j  2.+1.j  2.+1.j  2.+2.j]
    >>> print(zr)
    [ 1.  3.  4.]
    """

    z = atleast_1d(z)
    if z.size == 0:
        return z, z
    elif len(np.ndarray(z, dtype=np.complex).shape) != 1:
        raise ValueError('_cplxreal only accepts 1-D input')

    if tol is None:
        # Get tolerance from dtype of input
        tol = 100 * abs(7./3 - 4./3 - 1) #np.finfo((1.0 * z).dtype).eps

    # Sort by real part, magnitude of imaginary part (speed up further sorting)
    z = _lexsort(z)
    # Split reals from conjugate pairs
    real_indices = abs(z.imag) <= tol * abs(z)
    zr = z[real_indices].real

    if len(zr) == len(z):
        # Input is entirely real
        return np.array([],dtype=np.float), zr

    # Split positive and negative halves of conjugates
    inv_real_indices = np.array([not elem for elem in real_indices], dtype=np.bool)
    z = z[inv_real_indices]
    zp = z[z.imag > 0]
    zn = z[z.imag < 0]

    if len(zp) != len(zn):
        raise ValueError('Array contains complex value with no matching '
                         'conjugate.')

    # Find runs of (approximately) the same real part
    same_real = np.diff(zp.real) <= tol * abs(zp[:-1])
    same_real = np.array(same_real * 1, dtype=np.uint8)
    a = np.array([0], dtype=np.uint8)
    b = np.array([0], dtype=np.uint8)
    x = np.concatenate((a, same_real, b))
    diffs = numpy.diff(np.array(x, dtype=np.float))
    start = np.array((diffs > 0) * 1, dtype=np.uint16)
    stop = np.array((diffs < 0) * 1, dtype=np.uint16)
    run_starts = nonzero(start)[0]
    run_stops = nonzero(stop)[0]

    # Sort each run by their imaginary parts
    for i in range(len(run_starts)):
        start = int(run_starts[i])
        stop = int(run_stops[i]) + 1
        for chunk in (zp[start:stop], zn[start:stop]):
            a = 1


    # Check that negatives match positives
    if any(abs(zp - np.conjugate(zn)) > tol * abs(zn)):
        raise ValueError('Array contains complex value with no matching '
                         'conjugate.')

    # Average out numerical inaccuracy in real vs imag parts of pairs
    zc = (zp + np.conjugate(zn)) / 2

    return zc, zr

def zpk2sos(z, p, k, pairing='nearest'):
    """
    Return second-order sections from zeros, poles, and gain of a system

    Parameters
    ----------
    z : array_like
        Zeros of the transfer function.
    p : array_like
        Poles of the transfer function.
    k : float
        System gain.
    pairing : {'nearest', 'keep_odd'}, optional
        The method to use to combine pairs of poles and zeros into sections.
        See Notes below.

    Returns
    -------
    sos : ndarray
        Array of second-order filter coefficients, with shape
        ``(n_sections, 6)``. See `sosfilt` for the SOS filter format
        specification.

    See Also
    --------
    sosfilt

    Notes
    -----
    The algorithm used to convert ZPK to SOS format is designed to
    minimize errors due to numerical precision issues. The pairing
    algorithm attempts to minimize the peak gain of each biquadratic
    section. This is done by pairing poles with the nearest zeros, starting
    with the poles closest to the unit circle.

    *Algorithms*

    The current algorithms are designed specifically for use with digital
    filters. (The output coefficients are not correct for analog filters.)

    The steps in the ``pairing='nearest'`` and ``pairing='keep_odd'``
    algorithms are mostly shared. The ``nearest`` algorithm attempts to
    minimize the peak gain, while ``'keep_odd'`` minimizes peak gain under
    the constraint that odd-order systems should retain one section
    as first order. The algorithm steps and are as follows:

    As a pre-processing step, add poles or zeros to the origin as
    necessary to obtain the same number of poles and zeros for pairing.
    If ``pairing == 'nearest'`` and there are an odd number of poles,
    add an additional pole and a zero at the origin.

    The following steps are then iterated over until no more poles or
    zeros remain:

    1. Take the (next remaining) pole (complex or real) closest to the
       unit circle to begin a new filter section.

    2. If the pole is real and there are no other remaining real poles [#]_,
       add the closest real zero to the section and leave it as a first
       order section. Note that after this step we are guaranteed to be
       left with an even number of real poles, complex poles, real zeros,
       and complex zeros for subsequent pairing iterations.

    3. Else:

        1. If the pole is complex and the zero is the only remaining real
           zero*, then pair the pole with the *next* closest zero
           (guaranteed to be complex). This is necessary to ensure that
           there will be a real zero remaining to eventually create a
           first-order section (thus keeping the odd order).

        2. Else pair the pole with the closest remaining zero (complex or
           real).

        3. Proceed to complete the second-order section by adding another
           pole and zero to the current pole and zero in the section:

            1. If the current pole and zero are both complex, add their
               conjugates.

            2. Else if the pole is complex and the zero is real, add the
               conjugate pole and the next closest real zero.

            3. Else if the pole is real and the zero is complex, add the
               conjugate zero and the real pole closest to those zeros.

            4. Else (we must have a real pole and real zero) add the next
               real pole closest to the unit circle, and then add the real
               zero closest to that pole.

    .. [#] This conditional can only be met for specific odd-order inputs
           with the ``pairing == 'keep_odd'`` method.

    .. versionadded:: 0.16.0

    Examples
    --------

    Design a 6th order low-pass elliptic digital filter for a system with a
    sampling rate of 8000 Hz that has a pass-band corner frequency of
    1000 Hz. The ripple in the pass-band should not exceed 0.087 dB, and
    the attenuation in the stop-band should be at least 90 dB.

    In the following call to `signal.ellip`, we could use ``output='sos'``,
    but for this example, we'll use ``output='zpk'``, and then convert to SOS
    format with `zpk2sos`:

    >>> from scipy import signal
    >>> z, p, k = signal.ellip(6, 0.087, 90, 1000/(0.5*8000), output='zpk')

    Now convert to SOS format.

    >>> sos = signal.zpk2sos(z, p, k)

    The coefficients of the numerators of the sections:

    >>> sos[:, :3]
    array([[ 0.0014154 ,  0.00248707,  0.0014154 ],
           [ 1.        ,  0.72965193,  1.        ],
           [ 1.        ,  0.17594966,  1.        ]])

    The symmetry in the coefficients occurs because all the zeros are on the
    unit circle.

    The coefficients of the denominators of the sections:

    >>> sos[:, 3:]
    array([[ 1.        , -1.32543251,  0.46989499],
           [ 1.        , -1.26117915,  0.6262586 ],
           [ 1.        , -1.25707217,  0.86199667]])

    The next example shows the effect of the `pairing` option.  We have a
    system with three poles and three zeros, so the SOS array will have
    shape (2, 6). The means there is, in effect, an extra pole and an extra
    zero at the origin in the SOS representation.

    >>> z1 = np.array([-1, -0.5-0.5j, -0.5+0.5j])
    >>> p1 = np.array([0.75, 0.8+0.1j, 0.8-0.1j])

    With ``pairing='nearest'`` (the default), we obtain

    >>> signal.zpk2sos(z1, p1, 1)
    array([[ 1.  ,  1.  ,  0.5 ,  1.  , -0.75,  0.  ],
           [ 1.  ,  1.  ,  0.  ,  1.  , -1.6 ,  0.65]])

    The first section has the zeros {-0.5-0.05j, -0.5+0.5j} and the poles
    {0, 0.75}, and the second section has the zeros {-1, 0} and poles
    {0.8+0.1j, 0.8-0.1j}. Note that the extra pole and zero at the origin
    have been assigned to different sections.

    With ``pairing='keep_odd'``, we obtain:

    >>> signal.zpk2sos(z1, p1, 1, pairing='keep_odd')
    array([[ 1.  ,  1.  ,  0.  ,  1.  , -0.75,  0.  ],
           [ 1.  ,  1.  ,  0.5 ,  1.  , -1.6 ,  0.65]])

    The extra pole and zero at the origin are in the same section.
    The first section is, in effect, a first-order section.

    """
    # TODO in the near future:
    # 1. Add SOS capability to `filtfilt`, `freqz`, etc. somehow (#3259).
    # 2. Make `decimate` use `sosfilt` instead of `lfilter`.
    # 3. Make sosfilt automatically simplify sections to first order
    #    when possible. Note this might make `sosfiltfilt` a bit harder (ICs).
    # 4. Further optimizations of the section ordering / pole-zero pairing.
    # See the wiki for other potential issues.

    valid_pairings = ['nearest', 'keep_odd']
    if pairing not in valid_pairings:
        raise ValueError('pairing must be one of %s, not %s'
                         % (valid_pairings, pairing))
    if len(z) == len(p) == 0:
        return np.array([[k, 0., 0., 1., 0., 0.]])

    # ensure we have the same number of poles and zeros, and make copies

    if len(z) != len(p):
        if max(len(z) - len(p),0) > 0:  
            p = np.concatenate((p, np.zeros((max(len(z) - len(p), 0)),dtype=np.complex)))
        if max(len(p) - len(z),0) > 0:   
            z = np.concatenate((z, np.zeros((max(len(p) - len(z), 0)), dtype=np.complex)))

    n_sections = (max(len(p), len(z)) + 1) // 2
    sos = np.zeros((n_sections, 6))
    

    if len(p) % 2 == 1 and pairing == 'nearest':
        p = np.concatenate((p, np.array([0.],dtype=np.complex)))
        z = np.concatenate((z, np.array([0.],dtype=np.complex)))
    assert len(p) == len(z)

    z = np.array(z, dtype=np.complex)


    # Ensure we have complex conjugate pairs
    # (note that _cplxreal only gives us one element of each complex pair):
    cplx, rel = _cplxreal(p)
    if len(rel) > 0 and len(cplx) > 0:
        p = np.concatenate((cplx, np.array(rel, dtype=np.complex))) 
    else:
        p = cplx
   
    
    cplx, rel = _cplxreal(z)
    if len(rel) > 0 and len(cplx) > 0:
        z = np.concatenate((cplx, np.array(rel, dtype=np.complex))) 
    else:
        z = rel

    p_sos = np.zeros((n_sections, 2))
    z_sos = zeros_like(p_sos)

    for si in range(n_sections):
        # Select the next "worst" pole
        p1_idx = np.argmin(abs(1 - abs(p)))
        p1 = p[p1_idx]
        p = np.delete(p, p1_idx)
        # Pair that pole with a zero
        if isreal(p1) and np.sum([isreal(p)]) == 0:
            # Special case to set a first-order section
            z1_idx = _nearest_real_complex_idx(z, p1, 'real')
            z1 = z[z1_idx]
            z = np.delete(z, z1_idx)
            p2 = z2 = 0
        else:
            if not isreal(p1) and np.sum(isreal(z)) == 1:
                # Special case to ensure we choose a complex zero to pair
                # with so later (setting up a first-order section)
                z1_idx = _nearest_real_complex_idx(z, p1, 'complex')
                assert not isreal(z[z1_idx])
            else:
                # Pair the pole with the closest zero (real or complex)
                z1_idx = np.argmin(abs(p1 - z))
            z1 = z[z1_idx]
            z = np.delete(z, z1_idx)
            # Now that we have p1 and z1, figure out what p2 and z2 need to be
            if not isreal(p1):
                if not isreal(z1):  # complex pole, complex zero
                    p2 = np.conjugate(p1)
                    z2 = np.conjugate(z1)
                else:  # complex pole, real zero
                    p2 = np.conjugate(p1) #p1.conj()
                    z2_idx = _nearest_real_complex_idx(z, p1, 'real')
                    z2 = z[z2_idx]
                    assert isreal(z2)
                    z = np.delete(z, z2_idx)
            else:
                if not isreal(z1):  # real pole, complex zero
                    z2 = np.conjugate(z1)
                    p2_idx = _nearest_real_complex_idx(p, z1, 'real')
                    p2 = p[p2_idx]
                    assert isreal(p2)
                else:  # real pole, real zero
                    # pick the next "worst" pole to use
                    idx = nonzero(isreal(p))[0]
                    assert len(idx) > 0
 
                    a = abs(abs(p[idx[0]]) - 1)
                    a = np.array([a])
                    p2_idx = idx[np.argmin(a) - 1]
                    p2 = p[p2_idx]
                    # find a real zero to match the added pole
                    assert isreal(p2)
                    z2_idx = _nearest_real_complex_idx(z, p2, 'real')
                    z2 = z[z2_idx]
                    assert isreal(z2)
                    z = np.delete(z, z2_idx)

                p = np.delete(p, p2_idx)

        p_sos = np.array(p_sos, dtype=np.complex)
        p_sos[si] = np.array([p1, p2], dtype=np.complex)
        z_sos[si] = np.array([z1, z2], dtype=np.float)

    assert len(p) == len(z) == 0  # we've consumed all poles and zeros
    del p, z

    # Construct the system, reversing order so the "worst" are last
    p_sos = p_sos[::-1].reshape((n_sections, 2))
    z_sos = z_sos[::-1].reshape((n_sections, 2))

    gains = np.ones(n_sections, dtype=np.float)
    gains[0] = k
    for si in range(n_sections):
        x = zpk2tf(z_sos[si], p_sos[si], gains[si])
        sos[si] = np.concatenate(x)
    return sos

def lp2bp_zpk(z, p, k, wo=1.0, bw=1.0):
    r"""
    Transform a lowpass filter prototype to a bandpass filter.

    Return an analog band-pass filter with center frequency `wo` and
    bandwidth `bw` from an analog low-pass filter prototype with unity
    cutoff frequency, using zeros, poles, and gain ('zpk') representation.

    Parameters
    ----------
    z : array_like
        Zeros of the analog filter transfer function.
    p : array_like
        Poles of the analog filter transfer function.
    k : float
        System gain of the analog filter transfer function.
    wo : float
        Desired passband center, as angular frequency (e.g., rad/s).
        Defaults to no change.
    bw : float
        Desired passband width, as angular frequency (e.g., rad/s).
        Defaults to 1.

    Returns
    -------
    z : ndarray
        Zeros of the transformed band-pass filter transfer function.
    p : ndarray
        Poles of the transformed band-pass filter transfer function.
    k : float
        System gain of the transformed band-pass filter.

    See Also
    --------
    lp2lp_zpk, lp2hp_zpk, lp2bs_zpk, bilinear
    lp2bp

    Notes
    -----
    This is derived from the s-plane substitution

    .. math:: s \rightarrow \frac{s^2 + {\omega_0}^2}{s \cdot \mathrm{BW}}

    This is the "wideband" transformation, producing a passband with
    geometric (log frequency) symmetry about `wo`.

    .. versionadded:: 1.1.0

    """
    z = atleast_1d(z)
    p = atleast_1d(p)
    wo = float(wo)
    bw = float(bw)

    degree = _relative_degree(z, p)

    # Scale poles and zeros to desired bandwidth
    z_lp = []
    for x in z:
        z_lp.append(x * bw/2)
    z_lp = np.array(z_lp, dtype=np.complex)

    p_lp = []
    for x in p:
        p_lp.append(x * bw/2)
    p_lp = np.array(p_lp, dtype=np.complex)



    # Square root needs to produce complex result, not NaN


    # Duplicate poles and zeros and shift from baseband to +wo and -wo
    if len(z_lp) > 0:
        z_bp = np.concatenate((z_lp + np.sqrt(z_lp*z_lp - wo*wo, dtype=np.complex),
                            z_lp - np.sqrt(z_lp*z_lp - wo*wo, dtype=np.complex)))                 
        z_bp = append(z_bp, np.zeros(degree),dtype=np.complex)                    
    else:
         z_bp = np.zeros(degree, dtype=np.complex)

    p_bp = np.concatenate((p_lp + np.sqrt(p_lp*p_lp - wo*wo, dtype=np.complex),
                           p_lp - np.sqrt(p_lp*p_lp - wo*wo, dtype=np.complex)))


    # Move degree zeros to origin, leaving degree zeros at infinity for BPF
    
   
    # Cancel out gain change from frequency scaling
    k_bp = k * bw**degree

    return z_bp, p_bp, k_bp

def lp2bs_zpk(z, p, k, wo=1.0, bw=1.0):
    r"""
    Transform a lowpass filter prototype to a bandstop filter.

    Return an analog band-stop filter with center frequency `wo` and
    stopband width `bw` from an analog low-pass filter prototype with unity
    cutoff frequency, using zeros, poles, and gain ('zpk') representation.

    Parameters
    ----------
    z : array_like
        Zeros of the analog filter transfer function.
    p : array_like
        Poles of the analog filter transfer function.
    k : float
        System gain of the analog filter transfer function.
    wo : float
        Desired stopband center, as angular frequency (e.g., rad/s).
        Defaults to no change.
    bw : float
        Desired stopband width, as angular frequency (e.g., rad/s).
        Defaults to 1.

    Returns
    -------
    z : ndarray
        Zeros of the transformed band-stop filter transfer function.
    p : ndarray
        Poles of the transformed band-stop filter transfer function.
    k : float
        System gain of the transformed band-stop filter.

    See Also
    --------
    lp2lp_zpk, lp2hp_zpk, lp2bp_zpk, bilinear
    lp2bs

    Notes
    -----
    This is derived from the s-plane substitution

    .. math:: s \rightarrow \frac{s \cdot \mathrm{BW}}{s^2 + {\omega_0}^2}

    This is the "wideband" transformation, producing a stopband with
    geometric (log frequency) symmetry about `wo`.

    .. versionadded:: 1.1.0

    """
    z = atleast_1d(z)
    p = atleast_1d(p)
    wo = float(wo)
    bw = float(bw)

    degree = _relative_degree(z, p)

    # Invert to a highpass filter with desired bandwidth
    z_hp = (bw/2) / z
    p_hp = (bw/2) / p

    if z_hp == float('inf'):
        z_hp = []

    # Square root needs to produce complex result, not NaN
    z_hp = np.array(z_hp, dtype=np.complex)
    p_hp = np.array(p_hp, dtype=np.complex)

    if len(z_hp) > 0:
        # Duplicate poles and zeros and shift from baseband to +wo and -wo
        z_bs = np.concatenate((z_hp + np.sqrt(z_hp*z_hp - wo*wo, dtype=np.complex),
                            z_hp - np.sqrt(z_hp*z_hp - wo*wo, dtype=np.complex)))
    else:
        z_bs = np.array([], dtype=np.complex)               

    p_bs = np.concatenate((p_hp + np.sqrt(p_hp*p_hp - wo*wo, dtype=np.complex),
                           p_hp - np.sqrt(p_hp*p_hp - wo*wo, dtype=np.complex)))

    # Move any zeros that were at infinity to the center of the stopband
    z_bs = append(z_bs, np.full(degree, 1j*wo, dtype=np.complex))
    z_bs = append(z_bs, np.full(degree, -1j*wo, dtype=np.complex))

    # Cancel out gain change caused by inversion
    k_bs = k * (prod(-z) / prod(-p)).real

    return z_bs, p_bs, k_bs

def bilinear_zpk(z, p, k, fs):
    r"""
    Return a digital IIR filter from an analog one using a bilinear transform.

    Transform a set of poles and zeros from the analog s-plane to the digital
    z-plane using Tustin's method, which substitutes ``(z-1) / (z+1)`` for
    ``s``, maintaining the shape of the frequency response.

    Parameters
    ----------
    z : array_like
        Zeros of the analog filter transfer function.
    p : array_like
        Poles of the analog filter transfer function.
    k : float
        System gain of the analog filter transfer function.
    fs : float
        Sample rate, as ordinary frequency (e.g., hertz). No prewarping is
        done in this function.

    Returns
    -------
    z : ndarray
        Zeros of the transformed digital filter transfer function.
    p : ndarray
        Poles of the transformed digital filter transfer function.
    k : float
        System gain of the transformed digital filter.

    See Also
    --------
    lp2lp_zpk, lp2hp_zpk, lp2bp_zpk, lp2bs_zpk
    bilinear

    Notes
    -----
    .. versionadded:: 1.1.0

    Examples
    --------
    >>> from scipy import signal
    >>> import matplotlib.pyplot as plt

    >>> fs = 100
    >>> bf = 2 * np.pi * np.array([7, 13])
    >>> filts = signal.lti(*signal.butter(4, bf, btype='bandpass', analog=True,
    ...                                   output='zpk'))
    >>> filtz = signal.lti(*signal.bilinear_zpk(filts.zeros, filts.poles,
    ...                                         filts.gain, fs))
    >>> wz, hz = signal.freqz_zpk(filtz.zeros, filtz.poles, filtz.gain)
    >>> ws, hs = signal.freqs_zpk(filts.zeros, filts.poles, filts.gain,
    ...                           worN=fs*wz)
    >>> plt.semilogx(wz*fs/(2*np.pi), 20*np.log10(np.abs(hz).clip(1e-15)),
    ...              label=r'$|H_z(e^{j \omega})|$')
    >>> plt.semilogx(wz*fs/(2*np.pi), 20*np.log10(np.abs(hs).clip(1e-15)),
    ...              label=r'$|H(j \omega)|$')
    >>> plt.legend()
    >>> plt.xlabel('Frequency [Hz]')
    >>> plt.ylabel('Magnitude [dB]')
    >>> plt.grid()
    """
    
    z = atleast_1d(z)
    p = atleast_1d(p)

    degree = _relative_degree(z, p)

    fs2 = 2.0*fs

    # Bilinear transform the poles and zeros
    z_z = (fs2 + z) / (fs2 - z)
    p_z = (fs2 + p) / (fs2 - p)

    # Any zeros that were at infinity get moved to the Nyquist frequency
    a = -np.ones(degree) + 0j

    if len(a) > 0:
        z_z = append(z_z, a)

    # Compensate for gain change
    k_z = k * (prod(fs2 - z) / prod(fs2 - p)).real

    return z_z, p_z, k_z

def _nearest_real_complex_idx(fro, to, which):
    """Get the next closest real or complex element based on distance"""
    assert which in ('real', 'complex')
    a = np.array(abs(fro - to), dtype=np.float)
    order = np.argsort(a, axis=0)   # Differs from numpy  TODO
    fo = [fro[i] for i in order]
    sorted_array_list = [fro[i] for i in order]
    mask = isreal(np.array(sorted_array_list, dtype=np.float))
    if which == 'complex':
        mask = ~mask
    mask = np.array([mask], dtype=np.uint16)
    nzm = np.array(nonzero(mask)[0],dtype=np.int8)
    return order[nzm[0]]

def _relative_degree(z, p):
    """
    Return relative degree of transfer function from zeros and poles
    """
    degree = len(p) - len(z)
    if degree < 0:
        raise ValueError("Improper transfer function. "
                         "Must have at least as many poles as zeros.")
    else:
        return degree

def lp2lp_zpk(z, p, k, wo=1.0):
    r"""
    Transform a lowpass filter prototype to a different frequency.

    Return an analog low-pass filter with cutoff frequency `wo`
    from an analog low-pass filter prototype with unity cutoff frequency,
    using zeros, poles, and gain ('zpk') representation.

    Parameters
    ----------
    z : array_like
        Zeros of the analog filter transfer function.
    p : array_like
        Poles of the analog filter transfer function.
    k : float
        System gain of the analog filter transfer function.
    wo : float
        Desired cutoff, as angular frequency (e.g., rad/s).
        Defaults to no change.

    Returns
    -------
    z : ndarray
        Zeros of the transformed low-pass filter transfer function.
    p : ndarray
        Poles of the transformed low-pass filter transfer function.
    k : float
        System gain of the transformed low-pass filter.

    See Also
    --------
    lp2hp_zpk, lp2bp_zpk, lp2bs_zpk, bilinear
    lp2lp

    Notes
    -----
    This is derived from the s-plane substitution

    .. math:: s \rightarrow \frac{s}{\omega_0}

    .. versionadded:: 1.1.0

    """
    z = atleast_1d(z)
    p = atleast_1d(p)
    wo = float(wo)  # Avoid int wraparound

    degree = _relative_degree(z, p)

    # Scale all points radially from origin to shift cutoff frequency

    z_lp = []
    for x in z:
        z_lp.append(wo * x)
    z_lp = np.array(z_lp, dtype=np.complex)

    p_lp = []
    for x in p:
        p_lp.append(wo * x)
    p_lp = np.array(p_lp, dtype=np.complex)

    # Each shifted pole decreases gain by wo, each shifted zero increases it.
    # Cancel out the net change to keep overall gain the same
    k_lp = k * wo**degree

    return z_lp, p_lp, k_lp

def lp2hp_zpk(z, p, k, wo=1.0):
    r"""
    Transform a lowpass filter prototype to a highpass filter.

    Return an analog high-pass filter with cutoff frequency `wo`
    from an analog low-pass filter prototype with unity cutoff frequency,
    using zeros, poles, and gain ('zpk') representation.

    Parameters
    ----------
    z : array_like
        Zeros of the analog filter transfer function.
    p : array_like
        Poles of the analog filter transfer function.
    k : float
        System gain of the analog filter transfer function.
    wo : float
        Desired cutoff, as angular frequency (e.g., rad/s).
        Defaults to no change.

    Returns
    -------
    z : ndarray
        Zeros of the transformed high-pass filter transfer function.
    p : ndarray
        Poles of the transformed high-pass filter transfer function.
    k : float
        System gain of the transformed high-pass filter.

    See Also
    --------
    lp2lp_zpk, lp2bp_zpk, lp2bs_zpk, bilinear
    lp2hp

    Notes
    -----
    This is derived from the s-plane substitution

    .. math:: s \rightarrow \frac{\omega_0}{s}

    This maintains symmetry of the lowpass and highpass responses on a
    logarithmic scale.

    .. versionadded:: 1.1.0

    """
    z = atleast_1d(z)
    p = atleast_1d(p)
    
    wo = float(wo)

    degree = _relative_degree(z, p)

    # Invert positions radially about unit circle to convert LPF to HPF
    # Scale all points radially from origin to shift cutoff frequency
    z_hp = wo / z
    p_hp = wo / p

    if z_hp == float('inf'):
        z_hp = []

    # If lowpass had zeros at infinity, inverting moves them to origin.
    z_hp = append(z_hp, np.zeros(degree))

    # Cancel out gain change caused by inversion
    k_hp = k * (prod(-z) / prod(-p)).real

    return z_hp, p_hp, k_hp

def buttord(wp, ws, gpass, gstop, analog=False, fs=None):
    """Butterworth filter order selection.

    Return the order of the lowest order digital or analog Butterworth filter
    that loses no more than `gpass` dB in the passband and has at least
    `gstop` dB attenuation in the stopband.

    Parameters
    ----------
    wp, ws : float
        Passband and stopband edge frequencies.

        For digital filters, these are in the same units as `fs`. By default,
        `fs` is 2 half-cycles/sample, so these are normalized from 0 to 1,
        where 1 is the Nyquist frequency. (`wp` and `ws` are thus in
        half-cycles / sample.) For example:

            - Lowpass:   wp = 0.2,          ws = 0.3
            - Highpass:  wp = 0.3,          ws = 0.2
            - Bandpass:  wp = [0.2, 0.5],   ws = [0.1, 0.6]
            - Bandstop:  wp = [0.1, 0.6],   ws = [0.2, 0.5]

        For analog filters, `wp` and `ws` are angular frequencies (e.g., rad/s).
    gpass : float
        The maximum loss in the passband (dB).
    gstop : float
        The minimum attenuation in the stopband (dB).
    analog : bool, optional
        When True, return an analog filter, otherwise a digital filter is
        returned.
    fs : float, optional
        The sampling frequency of the digital system.

        .. versionadded:: 1.2.0

    Returns
    -------
    ord : int
        The lowest order for a Butterworth filter which meets specs.
    wn : ndarray or float
        The Butterworth natural frequency (i.e. the "3dB frequency"). Should
        be used with `butter` to give filter results. If `fs` is specified,
        this is in the same units, and `fs` must also be passed to `butter`.

    See Also
    --------
    butter : Filter design using order and critical points
    cheb1ord : Find order and critical points from passband and stopband spec
    cheb2ord, ellipord
    iirfilter : General filter design using order and critical frequencies
    iirdesign : General filter design using passband and stopband spec

    Examples
    --------
    Design an analog bandpass filter with passband within 3 dB from 20 to
    50 rad/s, while rejecting at least -40 dB below 14 and above 60 rad/s.
    Plot its frequency response, showing the passband and stopband
    constraints in gray.

    >>> from scipy import signal
    >>> import matplotlib.pyplot as plt

    >>> N, Wn = signal.buttord([20, 50], [14, 60], 3, 40, True)
    >>> b, a = signal.butter(N, Wn, 'band', True)
    >>> w, h = signal.freqs(b, a, np.logspace(1, 2, 500))
    >>> plt.semilogx(w, 20 * np.log10(abs(h)))
    >>> plt.title('Butterworth bandpass filter fit to constraints')
    >>> plt.xlabel('Frequency [radians / second]')
    >>> plt.ylabel('Amplitude [dB]')
    >>> plt.grid(which='both', axis='both')
    >>> plt.fill([1,  14,  14,   1], [-40, -40, 99, 99], '0.9', lw=0) # stop
    >>> plt.fill([20, 20,  50,  50], [-99, -3, -3, -99], '0.9', lw=0) # pass
    >>> plt.fill([60, 60, 1e9, 1e9], [99, -40, -40, 99], '0.9', lw=0) # stop
    >>> plt.axis([10, 100, -60, 3])
    >>> plt.show()

    """

    _validate_gpass_gstop(gpass, gstop)

    wp = atleast_1d(wp)
    ws = atleast_1d(ws)
    if fs is not None:
        if analog:
            raise ValueError("fs cannot be specified for an analog filter")
        wp = 2*wp/fs
        ws = 2*ws/fs

    filter_type = 2 * (len(wp) - 1)
    filter_type += 1
    if wp[0] >= ws[0]:
        filter_type += 1

    # Pre-warp frequencies for digital filter design
    if not analog:
        passb = math.tan(np.pi * wp / 2.0)
        stopb = math.tan(np.pi * ws / 2.0)
    else:
        passb = wp * 1.0
        stopb = ws * 1.0

    if filter_type == 1:            # low
        nat = stopb / passb
    elif filter_type == 2:          # high
        nat = passb / stopb
    elif filter_type == 3:          # stop
        wp0 = optimize.fminbound(band_stop_obj, passb[0], stopb[0] - 1e-12,   #TODO
                                 args=(0, passb, stopb, gpass, gstop,
                                       'butter'),
                                 disp=0)
        passb[0] = wp0
        wp1 = optimize.fminbound(band_stop_obj, stopb[1] + 1e-12, passb[1], #TODO
                                 args=(1, passb, stopb, gpass, gstop,
                                       'butter'),
                                 disp=0)
        passb[1] = wp1
        nat = ((stopb * (passb[0] - passb[1])) /
               (stopb ** 2 - passb[0] * passb[1]))
    elif filter_type == 4:          # pass
        nat = ((stopb ** 2 - passb[0] * passb[1]) /
               (stopb * (passb[0] - passb[1])))

    nat = min(abs(nat))

    GSTOP = 10 ** (0.1 * abs(gstop))
    GPASS = 10 ** (0.1 * abs(gpass))
    ord = int(math.ceil(math.log10((GSTOP - 1.0) / (GPASS - 1.0)) / (2 * math.log10(nat))))

    # Find the Butterworth natural frequency WN (or the "3dB" frequency")
    # to give exactly gpass at passb.
    try:
        W0 = (GPASS - 1.0) ** (-1.0 / (2.0 * ord))
    except ZeroDivisionError:
        W0 = 1.0
        print("Warning, order is zero...check input parameters.")

    # now convert this frequency back from lowpass prototype
    # to the original analog filter

    if filter_type == 1:  # low
        WN = W0 * passb
    elif filter_type == 2:  # high
        WN = passb / W0
    elif filter_type == 3:  # stop
        WN = numpy.empty(2, float)
        discr = math.sqrt((passb[1] - passb[0]) ** 2 +
                     4 * W0 ** 2 * passb[0] * passb[1])
        WN[0] = ((passb[1] - passb[0]) + discr) / (2 * W0)
        WN[1] = ((passb[1] - passb[0]) - discr) / (2 * W0)
        WN = numpy.sort(abs(WN))
    elif filter_type == 4:  # pass
        W0 = numpy.array([-W0, W0], float)
        WN = (-W0 * (passb[1] - passb[0]) / 2.0 +
              math.sqrt(W0 ** 2 / 4.0 * (passb[1] - passb[0]) ** 2 +
                   passb[0] * passb[1]))
        WN = numpy.sort(abs(WN))
    else:
        raise ValueError("Bad type: %s" % filter_type)

    if not analog:
        wn = (2.0 / np.pi) * math.arctan(WN)
    else:
        wn = WN

    if len(wn) == 1:
        wn = wn[0]

    if fs is not None:
        wn = wn*fs/2

    return ord, wn

def buttap(N):
    """Return (z,p,k) for analog prototype of Nth-order Butterworth filter.

    The filter will have an angular (e.g., rad/s) cutoff frequency of 1.

    See Also
    --------
    butter : Filter design function using this prototype

    """
    if abs(int(N)) != N:
        raise ValueError("Filter order must be a nonnegative integer")
    z = numpy.array([])
    m = numpy.arange(-N+1, N, 2)
    # Middle value is 0 to ensure an exactly real pole
    a = np.pi * m / (2 * N)
    b = []
    for x in a:
        b.append(1j * x)
    p = np.array(b, dtype=np.complex)
    p = -numpy.exp(p)
    k = 1
    return z, p, k

def butter_bandpass(lowcut, highcut, fs, order=5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    sos = butter(order, [low, high], analog=False, btype='band', output='sos')
    return sos

def butter_bandpass_filter(data, lowcut, highcut, fs, order=5):
    sos = butter_bandpass(lowcut, highcut, fs, order=order)
    y = spy.sosfilter(sos, data)
    return y

def fft(x):
    n = len(x)
    if n <= 1:
        return x
    even = fft(x[0::2])
    odd =  fft(x[1::2])
    return [even[m] + math.e**(-2j*math.pi*m/n)*odd[m] for m in range(n//2)] + [even[m] - math.e**(-2j*math.pi*m/n)*odd[m] for m in range(n//2)]

filter_dict = {'butter': [buttap, buttord],
               'butterworth': [buttap, buttord]
               }

band_dict = {'band': 'bandpass',
             'bandpass': 'bandpass',
             'pass': 'bandpass',
             'bp': 'bandpass',

             'bs': 'bandstop',
             'bandstop': 'bandstop',
             'bands': 'bandstop',
             'stop': 'bandstop',

             'l': 'lowpass',
             'low': 'lowpass',
             'lowpass': 'lowpass',
             'lp': 'lowpass',

             'high': 'highpass',
             'highpass': 'highpass',
             'h': 'highpass',
             'hp': 'highpass',
             }







