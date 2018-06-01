function s = cubic(k,tol)
%CUBIC Summary of this function goes here
%   Detailed explanation goes here

if nargin < 2
    tol = 1e-12;
end

fprintf('\ncubic()\n');


a = k(1);
b = k(2);
c = k(3);
d = k(4);

d0 = b^2 - 3*a*c;
d1 = 2*b^3 - 9*a*b*c + 27*a^2*d;
dl = (4*d0^3 - d1^2)/(27*a^2);

disp("d0,d1,dl")
disp(d0)
disp(d1)
disp(dl)

if dl ~= 0

    if d0 ~= 0

        C = ((d1 + (d1^2 - 4 *d0^3)^(1/2))/2)^(1/3);

    else

        C = d1^(1/3);

    end

    fprintf('C:\n %f%+fj\n', real(C), imag(C))


    u = [1 (-1 + 1i*3^(1/2))/2 (-1 - 1i*3^(1/2))/2];
    disp("u real")
    disp(real(u))
    disp("u imag")
    disp(imag(u))
    s = -(b + u*C + d0./(u*C))/(3*a);
    disp("s real")
    disp(real(s))
    disp("s imag")
    disp(imag(s))




else

    if d0 ~= 0

        s = [(9*a*d - b*c)/(2*d0) (4*a*b*c - 9*a^2*d - b^3)/(a*d0)];

    else

        s = -b/(3*a);

    end

end

for i = 1:length(s)
    if abs(imag(s(i))) < tol
        s(i) = real(s(i));
    end
end

disp("s_after_check_tol end of cubic")
disp(s)

end
